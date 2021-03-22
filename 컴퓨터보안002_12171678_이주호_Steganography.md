# Steganography 구현

#### 컴퓨터보안 002분반
#### 12171678 이주호

## 아이디어
아이디어 출현 과정
1. bmp 구조를 확인
2. bmp 이미지 내에 숨길 수 있는 공간 확인

### bmp 구조
마이크로소프트 공식 문서 [Windows Metafile Format](https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-wmf/4813e7fd-52d0-4f42-965f-228c8b7488d2?redirectedfrom=MSDN)에서 bmp 구조를 확인하였다.     
해당 문서(2.2.2 Strucutre Object)에 따르면 bitmap은 다음 이미지의 구조를 따른다.
![bmp-structure](./img/bmp-structure.jpg)

그 전에 bmp파일은 헤더 인포 위에 File Header로 bmp파일임을 명시하는 헤더를 가진다.    
그 아래 이미지에 대한 정보를 가지며 위 이미지에서 DIBHeaderInfo 부분을 차지한다.     
이 내용에서 이미지에 대한 폭, 높이, 픽셀당 bit 수 등에 대한 정보를 담고 있다.    

<br>

이미지 정보 헤더 아래에는 Colors라는 영역이 차지하는데, 공식문서에서는 Color Table이라 칭하고 있다.     
이 테이블에는 픽셀 컬러 변수를 정의하고 있다.    
![color-table](./img/color-table.jpg)

<br>

마지막으로 이미지를 표현하는 Pixel storage(BitmapBuffer)이다. 즉 이미지를 포함하고있는 버퍼이다.     
여기서 aData와 UndefinedSpace로 나누어지는데, 이는 주제에 대해서 중요하지 않으므로 넘어간다.    


### 숨길 수 있는 공간 확인
하지만 Pixel array를 메모리에서 불러올 때, 4배수로 불러와야 한다.    
만일 24 bitmap일 경우 Width = 1이면, 4바이트를 불러오고 3색상 rgb를 제외한 1바이트가 남게된다.    
또한, Width = 3이면, 3색상을 제외한 총 3바이트가 남게 된다.    
이를 Padding bytes라 하고, 여기에 데이터를 숨기면 되겠다고 생각을 했다.    

## bmpHeader.h
bmpHeader에 대한 내용은 [C 언어 코딩 도장](https://dojang.io/mod/page/view.php?id=703)을 참고했다.    
bmpHeader는 bmpFile 경로를 받아와서 해당 파일의 헤더 정보를 저장한다.    

- bmp File 헤더 정보를 저장한다.
    ```c++
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
    ```

<br>

- bmp Image 헤더 정보를 저장한다.
    ```c++
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
    ```

- 위 정보를 반환해줄 getter를 생성한다.
    ```c++
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
    ```

- `readHeader()`
    - 헤더 정보를 읽어서 저장한다.

## 