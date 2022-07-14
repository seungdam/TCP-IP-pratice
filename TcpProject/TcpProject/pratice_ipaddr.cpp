#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
int main() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(3, 2), &wsa) != 0)
		return 1;
	
	// ipv4 �ּ� ��ȯ
	char* ipv4 = "127.46.114.70";

	cout << "[���ڿ� �ּ�]" << ipv4 << endl;
	// inet_addr �Լ��� ����� ���ڿ� �ּҸ� ��Ʈ��ũ ����Ʈ ����
	cout << "[��Ʈ��ũ ����Ʈ ����]" << inet_addr(ipv4) << endl;
	
	// ����Ʈ ���ĵ� �ּ�
	IN_ADDR ipv4num; // ipv4 �ּ� ������ ���� ����ü
	ipv4num.s_addr = inet_addr(ipv4);
	cout << "[����Ʈ ����] -> [���ڿ�]" << inet_ntoa(ipv4num) << endl;

	// WSAStringToAddress �Լ� ���� ���ڿ� -> ����
	// WSAStringToAddress (���ڿ� ���� ip �ּ�, AddressFamily(AF_INET/6),IpProtocolInfo(NULL), IpAddress(ip�ּҸ� ������ ����ü), ������ ����)
	cout << endl;
	cout << "[���ڿ�]" << endl;
	char* ipv6 = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";
	cout << ipv6 << endl;
	
	cout << "[���ڿ�] -> [����]" << endl;
	SOCKADDR_IN6 ipv6num;
	int addrlen = sizeof(ipv6num); 
	WSAStringToAddress(ipv6, AF_INET6, NULL,(SOCKADDR*)&ipv6num, &addrlen);
	for (auto i : ipv6num.sin6_addr.u.Byte) {
		cout.width(2);
		cout.fill('0');
		cout << std::hex << (int)i;
	}

	// WSAAddressToString �Լ� ���� ����-> ���ڿ�
	cout << endl;
	cout << "[����] -> [���ڿ�]" << endl;
	char ipaddr[50];
	DWORD ipaddrlen = sizeof(ipaddr);
	WSAAddressToString((SOCKADDR*)&ipv6num, sizeof(ipv6num), NULL, ipaddr, &ipaddrlen);
	cout << ipaddr << endl;
	WSACleanup();
}