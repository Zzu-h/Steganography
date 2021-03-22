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
	// Padding�� offset���� ���� seekp �Լ��� pointer�� �ű��� write ����.
	// ���ڿ��� ���� �̹����� Padding�� �Ѿ ��� ���� �����͸� �� �ڿ� �̾����
	for (int i = 0; i < strSize; i++) {

		// �� �� padding�� �����͸� �� ���� ��� ���� padding ��ġ�� ã�Ƽ� �����͸� �ű�
		// ���ڿ� �ε����� �� ���� ������� ���� ��� Padding offset�� �ֱ������� �����Ѵ�.
		if (i< vacancy && !(i % paddingSize)) {
			// ������ Padding�� offset ��ġ�� ã�´�.
			offset = (rowSize * (i / paddingSize)) + originBmpHeader.getBfOffBits() + paddingOffset;
			target.seekp(offset, ios::beg);
		}

		// �� ������ �ϳ��� ���ڸ� ������
		target.write(&input[i], 1);

		// ���ڿ��� ������ ��� ���⸦ �����Ѵ�.
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

	// buffer ������ ���縦 ������ ����
	// �� �� �����ϴ��� ���
	// ������ �������� ����, �������� ���� ���� ����
	int cpyCount = originBmpHeader.getBfSize() / bufSize;
	char buffer[bufSize];

	// pointer�� ���������� �̵�
	origin.seekg(0, ios::beg);

	// ���ϳ��� ����
	for (int i = 0; i < cpyCount; i++){
		origin.read(buffer, bufSize);
		target.write(buffer, bufSize);
	}

	// �� ������ ���� �� ���� ������ ���� ����
	origin.read(buffer, originBmpHeader.getBfSize() % bufSize);
	target.write(buffer, originBmpHeader.getBfSize() % bufSize);

	return 0;
}

void encoding::calForEncoding() {
	// encoding�� �ʿ��� ���� ���
	rowSize = (((float)originBmpHeader.getBiWidth() * (float)originBmpHeader.getBiBitCount() + 31) / 32) * 4;
	paddingOffset = originBmpHeader.getBiWidth() * originBmpHeader.getBiBitCount() / 8;
	paddingSize = rowSize - paddingOffset;
	heightSize = originBmpHeader.getBiHeight();
	vacancy = paddingSize * heightSize;
}