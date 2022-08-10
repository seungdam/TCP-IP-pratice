#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

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

	SOCKET sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// 소켓구조체초기화
	sockaddr_in serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVERPORT);

	retval = connect(sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	sockaddr_in peeraddr; // 데이터 통신에 사용할 소켓주소구조체
	int addrlen;
	char buf[BUFSIZE + 1];
	int len;

	while (1) {
		cout << "chat: ";
		cin.getline(buf, BUFSIZE);
		len = strlen(buf);
		
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendto()");
			continue;
		}
		int sendbyte = retval;
		sockaddr_in localaddr;
		addrlen = sizeof(localaddr);
		ZeroMemory(&localaddr, sizeof(localaddr));
		retval = getsockname(sock, (sockaddr*)&localaddr, &addrlen);
		if (retval == SOCKET_ERROR) {
			err_display("getsockname()");
			continue;
		}
		printf("[UDP/%s:%d]: %d 바이트 전송 \n", inet_ntoa(localaddr.sin_addr), ntohs(localaddr.sin_port), sendbyte);

		//addrlen = sizeof(peeraddr);
		buf[retval] = '\0';
		retval = recv(sock, buf, BUFSIZE,0);
		if (retval == SOCKET_ERROR) {
			err_display("recvfrom()");
			continue;
		}
		printf("[UDP/%s:%d]: %s 전송 받음 \n", inet_ntoa(localaddr.sin_addr), ntohs(localaddr.sin_port), buf);

		/*if (memcmp(&peeraddr, &serveraddr, sizeof(peeraddr))) {
			cout << "잘못된 데이터" << endl;
		}*/
	}
	closesocket(sock);
	WSACleanup();

}