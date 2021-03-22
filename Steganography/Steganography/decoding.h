#pragma once

#include<iostream>
#include<fstream>
#include "bmpHeader.h"

# define MAX_LEN 2048

using namespace std;

class decoding{
private:
	bmpHeader SteganoBmpHeader;
	ifstream SteganoBmp;

	const char* path;

	// String data to be setganography
	char output[MAX_LEN + 1];

	size_t rowSize = 0;		// row�� ũ��
	size_t heightSize = 0;	// height�� ũ��
	size_t paddingSize = 0;	// �� row�� padding�� ũ��
	int paddingOffset = 0;	// ������ ���ۺ��� padding������ �ɼ�
	size_t vacancy = 0;		// bmp ���Ͽ��� �����͸� �Է��� �� �ִ� �� ������ ũ��.

	void calForDecoding();
public:
	decoding(const char*);
	errno_t doDecoding(char*);
};