#pragma once

#include "pch.h"
#include "bmpHeader.h"

class encoding
{
private:
	bmpHeader originBmpHeader;
	ifstream origin;
	ofstream target;

	const char* origin_Path;
	const char* target_Path;

	// String data to be setganography
	char input[MAX_LEN + 1] = { 0 };

	size_t rowSize = 0;		// row�� ũ��
	size_t heightSize = 0;	// height�� ũ��
	size_t paddingSize = 0;	// �� row�� padding�� ũ��
	int paddingOffset = 0;	// ������ ���ۺ��� padding������ �ɼ�
	size_t vacancy = 0;		// bmp ���Ͽ��� �����͸� �Է��� �� �ִ� �� ������ ũ��.

	errno_t copyBmp();
	void calForEncoding();
public:
	encoding(const char*, const char*);
	errno_t doEncoding(const char*);
};

