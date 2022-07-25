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

int main() {

	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(3, 2), &wsa) != 0) return 1;
	
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	int addrlen = sizeof(serveraddr);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVERPORT);
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	
	retval = connect(sock, (sockaddr*)&serveraddr, addrlen);
	if (retval == SOCKET_ERROR) err_quit("connect");
	
	int len;
	char buf[BUFSIZE + 1];
	char* testdata[]{
		"hello",
		"nice to meet you",
		"A lot of issue to say today",
		"Either I do",
	};

	for (auto i : testdata) {
		len = strlen(i);
		strncpy(buf, i, len);
		// 고정길이 데이터
		retval = send(sock, (char*)&len, sizeof(len), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		else if (retval == 0) break;
		// 가변길이 데이터 
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		else if (retval == 0) break;

		cout << "[TCP 클라이언트] " << retval << "바이트 전송 완료." << endl;
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}