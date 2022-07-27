#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

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
	int received;
	int left = len;
	char* ptr = buf;

	while (left > 0) {
		received = recv(s, buf, left, flags);

		if (received == SOCKET_ERROR) return received;
		else if (received == 0) break;

		left -= received;
		ptr += received;
	}

	return (len - left);
}

int main(int argc, char* argv[])
{
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(3, 2), &wsa) != 0) return 1;


	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	sockaddr_in serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	// bind()
	retval = bind(listen_sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	SOCKET client_sock;
	sockaddr_in clientaddr;
	ZeroMemory(&clientaddr, sizeof(clientaddr));
	int addrlen;
	
	char buf[BUFSIZE + 1];
	char id[BUFSIZE];
	char chat[BUFSIZE];

	strcpy(id, argv[1]);
	strcat(id, ": ");



	while (true) {
		client_sock = accept(listen_sock, (sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
		cout << "[TCP 서버]" << " 클라이언트 접속 : IP 주소: " << inet_ntoa(clientaddr.sin_addr) << "포트 번호: " << ntohs(clientaddr.sin_port) << endl;

		while (true) {
			retval = recvn(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) break;
			
			buf[retval] = '\0';
			cout << buf << endl;

			cout << "채팅: ";
			cin.getline(chat, BUFSIZE);
			strcpy(buf, id);
			strncat(buf, chat, BUFSIZE - strlen(buf));

			retval = send(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}
		closesocket(client_sock);
		cout << "접속 종료" << endl;
	}

	closesocket(listen_sock);
	WSACleanup();

}