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
- 생성자
    - 해당 bmp 파일을 불러올 경우 그 파일의 헤더를 분석하여 저장한다.

## encoding.h
- 멤버 변수
```c++
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
```
    - origin bmp파일의 헤더 정보와 해당 데이터들을 각각 origin과 originBmpHeader에 저장한다.
    - 그리고, Steganography를 적용할 파일 target을 생성한다.
    - 각 경로를 저장하고 입력 데이터 변수를 생성한다.
- 멤버 함수
    - doEncoding(const char*)
        - target에 Steganography를 적용한다.
    - bmpCopy()
        - target 이미지에 원본 이미지를 씌윈다.
    - calForEncoding()
        - 위 멤버변수 rowSize ~ vacancy를 계산한다.

### bmpCopy()
이 함수는 이미지를 복사하기 위한 함수이다.    
ifstream의 read와 ofstream의 write를 이용하여 이미지를 복사한다.     
하지만 한 번에 복사가 되기 힘드므로 buffer 단위로 나누어서 복사한다.     
```c++
const int bufSize = 216;

// buffer 단위로 복사를 진행할 예정
// 몇 번 복사하는지 계산
// 연산은 버림으로 진행, 나머지는 별도 복사 예정
int cpyCount = originBmpHeader.getBfSize() / bufSize;
char buffer[bufSize];

// pointer를 시작점으로 이동
origin.seekg(0, ios::beg);

// 파일내용 복사
for (int i = 0; i < cpyCount; i++){
	origin.read(buffer, bufSize);
	target.write(buffer, bufSize);
}

// 위 데이터 복사 후 남은 데이터 별도 복사
origin.read(buffer, originBmpHeader.getBfSize() % bufSize);
target.write(buffer, originBmpHeader.getBfSize() % bufSize);
```

### calForEncoding()
encoding을 위한 계산을 수행한다.     

![rowsize](./img/rowsize.jpg)

위 공식을 이용하여 rowSize를 계산하고, paddingOffset주소를 계산한다.    
paddingOffset주소는 `가로 크기 * 픽셀당 비트 수`에 색상의 비트 수를 나누어주면 첫 paddingOffset주소를 구할 수 있다.     
색상은 rgb 3가지 이므로 2^3 = 8이다. 따라서, `가로 크기 * 픽셀당 비트 수 / 8`이다.     
paddingSize는 rowSize에서 rgb가 포함하고 있는 공간을 제외한 값이다.     
따라서, rowSize - paddingOffset가 된다.     
Height는 그대로 저장하며, vacancy는 빈공간의 전체 크기이므로 한 줄당 Padding의 크기와 전체 높이의 크기를 곱한다.     

### doEndoding(const char*)
인코딩을 시작한다.     
인코딩을 위한 계산과 이미지 복사를 각 함수를 통해 수행한 후 텍스트를 입력한다.     
```c++
int offset;
// Padding의 offset값에 따라서 seekp 함수로 pointer를 옮긴후 write 진행.
// 문자열이 기존 이미지의 Padding을 넘어설 경우 남은 데이터를 그 뒤에 이어붙임
for (int i = 0; i < strSize; i++) {
	// 한 블럭 padding에 데이터를 다 썼을 경우 다음 padding 위치를 찾아서 포인터를 옮김
	// 문자열 인덱스가 빈 공간 사이즈보다 작을 경우 Padding offset을 주기적으로 갱신한다.
	if (i< vacancy && !(i % paddingSize)) {
		// 다음의 Padding의 offset 위치를 찾는다.
		offset = (rowSize * (i / paddingSize)) + originBmpHeader.getBfOffBits() + paddingOffset;
		target.seekp(offset, ios::beg);
	}
	// 빈 공간에 하나의 문자를 저장함
	target.write(&input[i], 1);
	// 문자열이 끝났을 경우 쓰기를 중지한다.
	if (input[i] == NULL)
		break;
}
```

- 문자열 길이만큼 Loop한다.
    - `if (i< vacancy && !(i % paddingSize))`
        - 만일 현재 입력한 개수가 paddingSize를 다 채웠다면, 그다음 padding offset주소로 이동한다.
            - 처음에 padding offset을 초기화하는 용도이기도 하다.
        - 하지만 문자열이 아직 남았음에도 이미 이미지의 빈 공간을 다 채웠을 경우 그다음 padding offset 주소로 옮기는 것은 무의미하다.
            - 따라서, 그 후에는 더 이상 옮기지 않는다.
    - `target.write(&input[i], 1)`
        - 빈 공간 하나에 문자 하나를 입력한다.

## decoding.h
- 멤버 변수
```c++
bmpHeader SteganoBmpHeader;
ifstream SteganoBmp;
const char* path;
// String data to be setganography
char output[MAX_LEN + 1];
size_t rowSize = 0;		// row의 크기
size_t heightSize = 0;	// height의 크기
size_t paddingSize = 0;	// 한 row당 padding의 크기
int paddingOffset = 0;	// 데이터 시작부터 padding까지의 옵셋
size_t vacancy = 0;		// bmp 파일에서 데이터를 입력할 수 있는 빈 공간의 크기.
```
    - 분석할 Steganography 파일의 헤더 정보를 저장하고, 해당 문자열을 읽어오기 위해 파일 데이터를 저장할 SteganoBmpHeader, SteganoBmp를 각각 생성한다.
    - encoding.h와 동일하게 출력할 문자열 변수와 padding을 연산하기 위한 각 변수를 생성한다.
- 멤버 함수
    - calForDecoding()
        - 위 멤버변수 rowSize ~ vacancy를 계산한다.
    - doDecoding(char*)
        - Steganography 파일의 문자열을 추출한다.

### calForDecoding()
calForEncoding()과 동일하다.

### doDecoding(char*)
인코딩과 반대로 paddingOffset주소를 옮겨가며 데이터를 추출한다.    

```c++
int offset;
for (int i = 0; i < MAX_LEN; i++) {
	if (i< vacancy && !(i % paddingSize)) {
		offset = (rowSize * (i / paddingSize)) + SteganoBmpHeader.getBfOffBits() + paddingOffset;
		SteganoBmp.seekg(offset, ios::beg);
	}
	SteganoBmp.read(output + i, 1);
	if (output[i] == NULL)
		break;
}
strcpy_s(outputStr, MAX_LEN, output);
```

인코딩 때 문자열이 이미지를 넘어설 경우 이미지 뒤에다 데이터를 입력했다.    
따라서, 디코딩 때 최대 문자열 길이만큼 Loop를 하며, 인코딩과 마찬가지로 Padding Offset 주소를 옮기며 데이터를 읽는다.    
중간에 끝을 알리는 NULL과 만났을 경우 Loop를 종료하며 읽은 문자열을 출력한다.

마지막으로 다시 main함수로 보내기 위해 읽었던 데이터들을 받아온 메모리 주소에 복사하고 종료한다.    

## main
매개변수로 `int argc, char* argv[]`를 받는다.    
argc는 데이터의 개수를 의미하며, argv는 실제적인 데이터이다.    
우리가 d나 e를 입력할경우 argc는 2개를 가져올 것이다.     
따라서, 만일 argc가 2가 아닌경우에 데이터를 입력하지 않았거나 더 많은 데이터를 가져왔음을 의미하므로 에러를 출력한다.    

그리고 argv를 통해 d인지 e인지 판별한다.     

```c++
if (argv[1][0] == 'e') {
	encoding encode("origin.bmp", "12171678_stego.bmp");
	char inputStr[MAX_LEN + 1];
	cout << "Please insert the text \n↓\n";
	cin.getline(inputStr, MAX_LEN);
	if (encode.doEncoding(inputStr))
		cout << "Encoding Failed." << endl;
}
else if (argv[1][0] == 'd') {
	char outputStr[MAX_LEN + 1];
	decoding decode("12171678_stego.bmp");
	if (decode.doDecoding(outputStr))
		cout << "Decoding Failed" << endl;
	else
		cout << outputStr << endl;
}
else {
	cout << "Please select 'd' or 'e'." << endl;
}
```

## 예제 입출력 테스트
1. ![Example1](./img/Example1.jpg)

<br>

2. ![Example2](./img/Example2.jpg)