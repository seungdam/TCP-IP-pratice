#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 50

using std::cout;
using std::endl;
using std::cin;


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
	cout << "[" << msg << "] : " << (char*)lpMsgBuf << endl;
	LocalFree(lpMsgBuf);
}

int recvn(SOCKET s, char* buf, int len, int flags) {
	int received; // 받은 데이터양
	int left = len;
	char* ptr = buf;
	while (left > 0) {
		received = recv(s, buf, left, flags);
		if (received == SOCKET_ERROR) {
			return received;
		}
		else if (received == 0)
			break;

		left -= received;
		ptr  += received;
	}

	return (len - left);
}

int main(int argc, char* argv[])
{
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(3, 2), &wsa) != 0) return 1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	sockaddr_in serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = connect(sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	char buf[BUFSIZE + 1];
	char id[BUFSIZE];
	char chat[BUFSIZE];

	strcpy(id, argv[1]);
	strcat(id, ": ");

	while (true) {

		cout << "채팅: ";
		cin.getline(chat, BUFSIZE);
		strcpy(buf, id);
		strncat(buf, chat, BUFSIZE - strlen(buf));
		retval = send(sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		else if (retval == 0) break;
		
		retval = recvn(sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		cout << endl;
		cout << buf << endl;
	}

	WSACleanup();
	closesocket(sock);

}