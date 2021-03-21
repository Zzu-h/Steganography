#include "pch.h"
#include "bmpHeader.h"

bmpHeader::bmpHeader(const string& _path) :path(_path){
	//BMP File Header

	bfType = 0;
	bfSize = 0;
	bfReserved1 = 0;
	bfReserved2 = 0;
	bfOffBits = 0;

	//BMP Image Header

	biSize = 0;
	biWidth = 0;
	biHeight = 0;
	biPlanes = 0;
	biBitCount = 0;
	biCompression = 0;
	biSizeImage = 0;
	biXPelsPerMeter = 0;
	biYPelsPerMeter = 0;
	biClrUsed = 0;
	biClrImportant = 0;

	isRead = false;
}

void bmpHeader::readHeader(){
	bmpImage.open(path, ios::in | ios::binary);

	if (!bmpImage.is_open()) {
		cout << "file has failed open" << endl;
		return;
	}

	//BMP File Header

	bmpImage.read((char*)&bfType, bfType_SIZE);
	bmpImage.read((char*)&bfSize, bfSize_SIZE);
	bmpImage.read((char*)&bfReserved1, bfReserved1_SIZE);
	bmpImage.read((char*)&bfReserved2, bfReserved2_SIZE);
	bmpImage.read((char*)&bfOffBits, bfOffBits_SIZE);

	//BMP Image Header			 

	bmpImage.read((char*)&biSize, biSize_SIZE);
	bmpImage.read((char*)&biWidth, biWidth_SIZE);
	bmpImage.read((char*)&biHeight, biHeight_SIZE);
	bmpImage.read((char*)&biPlanes, biPlanes_SIZE);
	bmpImage.read((char*)&biBitCount, biBitCount_SIZE);
	bmpImage.read((char*)&biCompression, biCompression_SIZE);
	bmpImage.read((char*)&biSizeImage, biSizeImage_SIZE);
	bmpImage.read((char*)&biXPelsPerMeter, biXPelsPerMeter_SIZE);
	bmpImage.read((char*)&biYPelsPerMeter, biYPelsPerMeter_SIZE);
	bmpImage.read((char*)&biClrUsed, biClrUsed_SIZE);
	bmpImage.read((char*)&biClrImportant, biClrImportant_SIZE);

	isRead = true;

	if (bmpImage.is_open())
		bmpImage.close();
}