#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#define SERVER_PORT 9000
#define BUFSIZE 256

SOCKET c_sock; // 클라이언트 소켓
WSAOVERLAPPED c_over; // overlapped 구조체
WSABUF c_wsabuf[1];
char c_msg[BUFSIZE];


// Overlapped I/O CALLBACK 모델
int main() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

	WSACleanup();
}