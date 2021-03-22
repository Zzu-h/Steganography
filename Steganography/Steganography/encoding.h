#pragma once

#include<iostream>
#include<fstream>
#include "bmpHeader.h"

# define MAX_LEN 2048

using namespace std;


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

	size_t rowSize = 0;		// row의 크기
	size_t heightSize = 0;	// height의 크기
	size_t paddingSize = 0;	// 한 row당 padding의 크기
	int paddingOffset = 0;	// 데이터 시작부터 padding까지의 옵셋
	size_t vacancy = 0;		// bmp 파일에서 데이터를 입력할 수 있는 빈 공간의 크기.

	errno_t copyBmp();
	void calForEncoding();
public:
	encoding(const char*, const char*);
	errno_t doEncoding(const char*);
};

