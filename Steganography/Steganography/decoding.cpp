#include "decoding.h"

decoding::decoding(const char* stegoPath)
	:SteganoBmpHeader(stegoPath) {
	this->path = stegoPath;
}
errno_t decoding::doDecoding(char* outputStr){

	calForDecoding();

	SteganoBmp.open(path, ios::in | ios::binary);

	if (!SteganoBmp.is_open()) {
		cout << "BMP File has failed open." << endl;
		return 1;
	}

	int offset;
	// Padding의 offset값에 따라서 seekg 함수로 pointer를 옮긴후 read 진행.
	// 문자열이 기존 이미지의 Padding을 넘어설 경우 남은 데이터를 읽기 위해 문자열 전체를 읽는다.
	// 최대 문자열 길이는 MAX_LEN이므로 전체를 LOOP한다. 그리고 NULL값(== 문자열 끝)을 마주하면 LOOP 종료
	for (int i = 0; i < MAX_LEN; i++) {
		
		// 문자열 인덱스가 빈 공간 사이즈보다 작을 경우 Padding offset을 주기적으로 갱신한다.
		if (i< vacancy && !(i % paddingSize)) {
			// 다음의 Padding의 offset 위치를 찾는다.
			offset = (rowSize * (i / paddingSize)) + SteganoBmpHeader.getBfOffBits() + paddingOffset;
			SteganoBmp.seekg(offset, ios::beg);
		}

		SteganoBmp.read(output + i, 1);

		if (output[i] == NULL)
			break;
	}

	strcpy_s(outputStr, MAX_LEN, output);

	SteganoBmp.close();

	return 0;
}

void decoding::calForDecoding(){
	SteganoBmpHeader.readHeader();

	rowSize = (((float)SteganoBmpHeader.getBiWidth() * (float)SteganoBmpHeader.getBiBitCount() + 31) / 32) * 4;
	paddingOffset = SteganoBmpHeader.getBiWidth() * SteganoBmpHeader.getBiBitCount() / 8;
	paddingSize = rowSize - paddingOffset;
	heightSize = SteganoBmpHeader.getBiHeight();
	vacancy = paddingSize * heightSize;
}