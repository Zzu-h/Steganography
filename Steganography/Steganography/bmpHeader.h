#pragma once

#include "pch.h"

#define OPEN_ERR	-1

//BMP File Header

#define bfType_SIZE			2
#define bfSize_SIZE			4
#define bfReserved1_SIZE	2
#define bfReserved2_SIZE	2
#define bfOffBits_SIZE		4

#define bfType_OFFSET		0
#define bfSize_OFFSET		2
#define bfReserved1_OFFSET	6
#define bfReserved2_OFFSET	8
#define bfOffBits_OFFSET	10

//BMP Image Header

#define biSize_SIZE				4
#define biWidth_SIZE			4
#define biHeight_SIZE			4
#define biPlanes_SIZE			2
#define biBitCount_SIZE			2
#define biCompression_SIZE		4
#define	biSizeImage_SIZE		4
#define	biXPelsPerMeter_SIZE	4
#define biYPelsPerMeter_SIZE	4
#define biClrUsed_SIZE			4
#define biClrImportant_SIZE		4

#define biSize_OFFSET			14
#define biWidth_OFFSET			18
#define biHeight_OFFSET			22
#define biPlanes_OFFSET			26
#define biBitCount_OFFSET		28
#define biCompression_OFFSET	30
#define	biSizeImage_OFFSET		34
#define	biXPelsPerMeter_OFFSET	38
#define biYPelsPerMeter_OFFSET	42
#define biClrUsed_OFFSET		46
#define biClrImportant_OFFSET	50

class bmpHeader
{

private:

	//BMP File Header

	unsigned short bfType;
	unsigned int bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int bfOffBits;

	//BMP Image Header

	unsigned int biSize;
	int biWidth;
	int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	int	biXPelsPerMeter;
	int biYPelsPerMeter;
	unsigned int biClrUsed;
	int biClrImportant;

	// file stream of BMP image file
	ifstream bmpImage;

	// path of image file
	string path;

	bool isRead;

	// Method to read header of bmp file


public:
	bmpHeader(const string& _path);
	bmpHeader() = delete;
	const bool is_open() { return bmpImage.is_open(); }
	const bool is_read() { return isRead; }
	void readHeader();


	//BMP File Header

	const unsigned short getBfType() { return bfType; }
	const unsigned int getBfSize() { return bfSize; }
	const unsigned short getBfReserved1() { return bfReserved2; }
	const unsigned short getBfReserved2() { return bfReserved2; }
	const unsigned int getBfOffBits() { return bfOffBits; }

	//BMP Image Header

	const unsigned int getBiSize() { return biSize; }
	const int getBiWidth() { return biWidth; }
	const int getBiHeight() { return biHeight; }
	const unsigned short getBiPlanes() { return biPlanes; }
	const unsigned short getBiBitCount() { return biBitCount; }
	const unsigned int getBiCompression() { return biCompression; }
	const unsigned int getBiSizeImage() { return biSizeImage; }
	const int getBiXPelsPerMeter() { return biXPelsPerMeter; }
	const int getBiYPelsPerMeter() { return biYPelsPerMeter; }
	const unsigned int getBiClrUsed() { return biClrUsed; }
	const int getBiClrImportant() { return biClrImportant; }

};

