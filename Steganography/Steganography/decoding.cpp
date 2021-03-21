#include "pch.h"
#include "decoding.h"

decoding::decoding(const char* _path)
	:SteganoBmpHeader(_path) {
	this->path = _path;
}
errno_t decoding::doDecoding(char* outputStr){

	calForDecoding();

	SteganoBmp.open(path, ios::in | ios::binary);

	if (!SteganoBmp.is_open()) {
		cout << "BMP File has failed open." << endl;
		return 1;
	}

	int offset;
	for (int i = 0; i < vacancy; i++) {
		if (!(i % paddingSize)) {
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