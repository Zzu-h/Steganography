#include "encoding.h"

encoding::encoding(const char* _origin, const char* _target)
	: originBmpHeader(_origin){
	this->origin_Path = _origin;
	this->target_Path = _target;
}

errno_t encoding::doEncoding(const char* inputStr){
	
	calForEncoding();
	strcpy_s(input, MAX_LEN, inputStr);

	origin.open(origin_Path, ios::in | ios::binary);
	target.open(target_Path, ios::out | ios::binary);

	if (!origin.is_open() || !target.is_open()) {
		cout << "BMP Files was not open. \nPlease Check the origin or target" << endl;
		return 1;
	}

	if (bmpCopy())
		return 1;
	

	size_t strSize = strlen(inputStr);

	int offset;
	// Padding의 offset값에 따라서 seekp 함수로 pointer를 옮긴후 write 진행.
	// 문자열이 기존 이미지의 Padding을 넘어설 경우 남은 데이터를 그 뒤에 이어붙임
	for (int i = 0; i < strSize; i++) {

		// 한 블럭 padding에 데이터를 다 썼을 경우 다음 padding 위치를 찾아서 포인터를 옮김
		// 문자열 인덱스가 빈 공간 사이즈보다 작을 경우 Padding offset을 주기적으로 갱신한다.
		if (i< vacancy && !(i % paddingSize)) {
			// 다음의 Padding의 offset 위치를 찾는다.
			offset = (rowSize * (i / paddingSize)) + originBmpHeader.getBfOffBits() + paddingOffset;
			target.seekp(offset, ios::beg);
		}

		// 빈 공간에 하나의 문자를 저장함
		target.write(&input[i], 1);

		// 문자열이 끝났을 경우 쓰기를 중지한다.
		if (input[i] == NULL)
			break;
	}

	origin.close();
	target.close();

	return 0;
}

errno_t encoding::bmpCopy(){
	if (!origin.is_open() || !target.is_open()) {
		cout << "Copy Fail." << endl;
		return 1;
	}

	const int bufSize = 216;

	// buffer 단위로 복사를 진행할 예정
	// 몇 번 복사하는지 계산
	// 연산은 버림으로 진행, 나머지는 별도 복사 예정
	int cpyCount = originBmpHeader.getBfSize() / bufSize;
	char buffer[bufSize];

	// pointer를 시작점으로 이동
	origin.seekg(0, ios::beg);

	// 파일내용 복사
	for (int i = 0; i < cpyCount; i++){
		origin.read(buffer, bufSize);
		target.write(buffer, bufSize);
	}

	// 위 데이터 복사 후 남은 데이터 별도 복사
	origin.read(buffer, originBmpHeader.getBfSize() % bufSize);
	target.write(buffer, originBmpHeader.getBfSize() % bufSize);

	return 0;
}

void encoding::calForEncoding() {
	// encoding에 필요한 값들 계산
	rowSize = (((float)originBmpHeader.getBiWidth() * (float)originBmpHeader.getBiBitCount() + 31) / 32) * 4;
	paddingOffset = originBmpHeader.getBiWidth() * originBmpHeader.getBiBitCount() / 8;
	paddingSize = rowSize - paddingOffset;
	heightSize = originBmpHeader.getBiHeight();
	vacancy = paddingSize * heightSize;
}