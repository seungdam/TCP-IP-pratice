#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
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
	if (*(char*)&tmp == 0x78) {
		cout << "��Ʋ" << endl;
		return true;
	}
	cout << "�ƴ�" << endl;
	return false;
}

bool isBigEndian() {
	int tmp = 0x12345678;
	char* ptr = (char*)&tmp;
	if (*(char*)&tmp == 0x12) {
		cout << "��" << endl;
		return true;
	}
	cout << "�ƴ�" << endl;
	return false;
}

int main() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(3, 2), &wsa) != 0)
		return 0;
	// �������� 1
	isLittleEndian();
	isBigEndian();

	// �������� 2
	u_short x1 = 0x1234;// unsigned short
	u_long  y1 = 0x12345678;// unsigned long

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	u_short x2;
	u_long y2;
	cout << "[ȣ��Ʈ] --> [��Ʈ��ũ] ����Ʈ ����" << endl;
	WSAHtons(s, x1, &x2);
	WSAHtonl(s, y1, &y2);
	cout << "0x" << std::hex << x1 << " -> " << "0x" << x2 << endl;
	cout << "0x" << std::hex << y1 << " -> " << "0x" << y2 << endl;

	cout << endl;
	cout << "�ٽ� ��ȯ" << endl;
	WSANtohs(s, x2, &x2);
	WSANtohl(s, y2, &y2);
	cout << "0x" << std::hex << x2 << endl;
	cout << "0x" << std::hex << y2 << endl;

	// �������� 3
	char dn[256];
	ZeroMemory(&dn,sizeof(dn));
	std::cin >> dn;

	hostent* ptr = gethostbyname(dn);
	for(int i = 0; ptr->h_aliases[i] != NULL; ++i)
		cout << ptr->h_aliases[i] << endl;
	for (int i = 0; ptr->h_addr_list[i] != NULL; ++i)
	cout << inet_ntoa(*(IN_ADDR*)ptr->h_addr_list) << endl;
	WSACleanup();
}