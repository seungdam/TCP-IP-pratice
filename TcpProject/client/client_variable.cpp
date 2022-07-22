#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#define SERVERIP "0:0:0:0:0:0:0:1"
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
		received = recv(s, buf, len, flags);
		if (received == SOCKET_ERROR) {
			return received;
		}
		else if (received == 0)
			break;

		left -= received;
		ptr += received;
	}

	return (len - left);
}

int main(int argc, char* argv) {

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(3, 2), &wsa) != 0) return 1;

	int retval;
	SOCKET sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	sockaddr_in6 serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));

	serveraddr.sin6_family = AF_INET6;
	int addrlen = sizeof(serveraddr);
	WSAStringToAddress(SERVERIP, AF_INET6, NULL, (sockaddr*)&serveraddr, &addrlen);
	serveraddr.sin6_port = htons(SERVERPORT);


	// wsastringtoaddress 는 주소로 변환해 알아서 적용시켜줌.


	retval = connect(sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	char buf[BUFSIZE + 1];
	char* tmpTxt[]{
		"Hellow",
		"Nice to meet you",
		"A lot of issue to say",
		"Either I do",
	};
	int len;
	for (auto i : tmpTxt) {
		len = strlen(i);
		strncpy(buf, i, strlen(i));
		buf[len++] = '\n';
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		else if (retval == 0) {
			break;
		}
		cout << "[TCP 클라이언트] " << retval << "바이트 전송완료" << endl;
	}

	cout << "[TCP 클라이언트] 종료" << endl;
	closesocket(sock);
	WSACleanup();
}