#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#define SERVER_PORT 9000
#define BUFSIZE 256

SOCKET c_sock; // Ŭ���̾�Ʈ ����
WSAOVERLAPPED c_over; // overlapped ����ü
WSABUF c_wsabuf[1];
char c_msg[BUFSIZE];


// Overlapped I/O CALLBACK ��
int main() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

	WSACleanup();
}