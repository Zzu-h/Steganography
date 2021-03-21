#include "pch.h"
#include "encoding.h"
#include "decoding.h"

int main(int argc, char* argv[]) {

	if (argc != 2) {
		cout << "Please select 'd' or 'e'." << endl;
		exit(1);
	}

	if (argv[1][0] == 'e') {
		encoding encode("origin.bmp", "12171678_stego.bmp");
		char inputStr[MAX_LEN + 1];

		cout << "Please insert the text \n";
		cin.getline(inputStr, MAX_LEN);

		// doEncoding �Լ� ȣ��. �ݵ�� �Է��� ���ڿ��� �Բ� �ѱ� ��. ũ��� MAX_LEN����
		if (encode.doEncoding(inputStr))
			cout << "Encoding Failed." << endl;
	}
	else if (argv[1][0] == 'd') {

		char outputStr[MAX_LEN + 1];
		decoding decode("12171678_stego.bmp");

		// doDecoding �Լ� ȣ��. �ݵ�� ����� ���ڿ��� �Բ� �ѱ� ��. ũ��� MAX_LEN����
		if (decode.doDecoding(outputStr))
			cout << "Decoding Failed" << endl;
		else
			cout << outputStr << endl;
	}
	else {
		cout << "Please select 'd' or 'e'." << endl;
	}
	
	return 0;
}