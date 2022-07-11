#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

using std::cout;
using std::endl;

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

	WSACleanup();
}