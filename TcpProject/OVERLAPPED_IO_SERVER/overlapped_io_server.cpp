#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#define SERVER_PORT 9000
#define BUFSIZE 256

// ���� ��� ����� ���� �Լ���
void err_quit(char* msg) {
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::cout << "[" << msg << "] : " << (char*)lpMsgBuf << std::endl;
	LocalFree(lpMsgBuf);
}

SOCKET c_sock; // Ŭ���̾�Ʈ ����
WSAOVERLAPPED c_over; // overlapped ����ü
WSABUF c_wsabuf[1];
char c_msg[BUFSIZE];


// Overlapped I/O CALLBACK ��
int main() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

	SOCKET s_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED); // overlapped �����ϴ� ���� ����
	if (INVALID_SOCKET == s_sock) err_quit("WSAsocket()");
	
	sockaddr_in serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);

	int retval;
	retval = bind(s_sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == retval) err_quit("bind()");


	WSACleanup();
}