#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <chrono>

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

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	sockaddr_in serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// 수신한 데이터가 온 클라이언트 정보
	sockaddr_in clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	while (1) {
		addrlen = sizeof(clientaddr);
		retval = recvfrom(sock, buf,BUFSIZE,0,(sockaddr*)&clientaddr,&addrlen); // 그냥 받은 데이터를 버퍼에 넣고 그 데이터를 보낸 클라이언트 정보를 저장
		if (retval == SOCKET_ERROR) {
			err_display("recvfrom()");
			continue;
		}
		buf[retval] = '\0';
		cout << "[UDP: " << inet_ntoa(clientaddr.sin_addr) << ": " << ntohs(clientaddr.sin_port) << "]: " << buf << endl;
		if (strcmp(buf, "시간") == 0) {
			auto now = std::chrono::system_clock::now();
			time_t mt = std::chrono::system_clock::to_time_t(now);
			strcpy(buf, std::ctime(&mt));

			retval = sendto(sock, buf, strlen(buf), 0, (sockaddr*)&clientaddr, sizeof(clientaddr));
			if (retval == SOCKET_ERROR) {
				err_display("sendto()");
				continue;
			}
		}
		else {
			retval = sendto(sock, buf, strlen(buf), 0, (sockaddr*)&clientaddr, sizeof(clientaddr));
			if (retval == SOCKET_ERROR) {
				err_display("sendto()");
				continue;
			}
		}
	}

	closesocket(sock);
	WSACleanup();

}