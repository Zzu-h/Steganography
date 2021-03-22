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
	// Padding�� offset���� ���� seekg �Լ��� pointer�� �ű��� read ����.
	// ���ڿ��� ���� �̹����� Padding�� �Ѿ ��� ���� �����͸� �б� ���� ���ڿ� ��ü�� �д´�.
	// �ִ� ���ڿ� ���̴� MAX_LEN�̹Ƿ� ��ü�� LOOP�Ѵ�. �׸��� NULL��(== ���ڿ� ��)�� �����ϸ� LOOP ����
	for (int i = 0; i < MAX_LEN; i++) {
		
		// ���ڿ� �ε����� �� ���� ������� ���� ��� Padding offset�� �ֱ������� �����Ѵ�.
		if (i< vacancy && !(i % paddingSize)) {
			// ������ Padding�� offset ��ġ�� ã�´�.
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