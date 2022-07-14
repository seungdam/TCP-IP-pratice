#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

bool isLittleEndian() {
	int tmp = 0x12345678;
	char* ptr = (char*)&tmp;
	cout <<  std::hex << (int)ptr[0] << endl;
	cout <<  std:: hex << (int)ptr[1] << endl;
	cout <<  std:: hex << (int)ptr[2] << endl;
	cout <<  std::hex << (int)ptr[3] << endl;
	if (*(char*)&tmp == 0x78) {
		cout << "리틀" << endl;
		return true;
	}
	cout << "아님" << endl;
	return false;
}

bool isBigEndian() {
	int tmp = 0x12345678;
	char* ptr = (char*)&tmp;
	cout << std::hex << (int)ptr[0] << endl;
	cout << std::hex << (int)ptr[1] << endl;
	cout << std::hex << (int)ptr[2] << endl;
	cout << std::hex << (int)ptr[3] << endl;
	if (*(char*)&tmp == 0x12) {
		cout << "빅" << endl;
		return true;
	}
	cout << "아님" << endl;
	return false;
}

int main() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(3, 2), &wsa) != 0)
		return 0;
	isLittleEndian();
	isBigEndian();
	WSACleanup();
}