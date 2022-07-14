#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
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
	
	// ipv4 주소 변환
	char* ipv4 = "127.46.114.70";

	cout << "[문자열 주소]" << ipv4 << endl;
	// inet_addr 함수를 사용해 문자열 주소를 네트워크 바이트 정렬
	cout << "[네트워크 바이트 정렬]" << inet_addr(ipv4) << endl;
	
	// 바이트 정렬된 주소
	IN_ADDR ipv4num; // ipv4 주소 저장을 위한 구조체
	ipv4num.s_addr = inet_addr(ipv4);
	cout << "[바이트 정렬] -> [문자열]" << inet_ntoa(ipv4num) << endl;

	// WSAStringToAddress 함수 연습 문자열 -> 숫자
	// WSAStringToAddress (문자열 형식 ip 주소, AddressFamily(AF_INET/6),IpProtocolInfo(NULL), IpAddress(ip주소를 저장할 구조체), 버퍼의 길이)
	cout << endl;
	cout << "[문자열]" << endl;
	char* ipv6 = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";
	cout << ipv6 << endl;
	
	cout << "[문자열] -> [숫자]" << endl;
	SOCKADDR_IN6 ipv6num;
	int addrlen = sizeof(ipv6num); 
	WSAStringToAddress(ipv6, AF_INET6, NULL,(SOCKADDR*)&ipv6num, &addrlen);
	for (auto i : ipv6num.sin6_addr.u.Byte) {
		cout.width(2);
		cout.fill('0');
		cout << std::hex << (int)i;
	}

	// WSAAddressToString 함수 연습 숫자-> 문자열
	cout << endl;
	cout << "[숫자] -> [문자열]" << endl;
	char ipaddr[50];
	DWORD ipaddrlen = sizeof(ipaddr);
	WSAAddressToString((SOCKADDR*)&ipv6num, sizeof(ipv6num), NULL, ipaddr, &ipaddrlen);
	cout << ipaddr << endl;
	WSACleanup();
}