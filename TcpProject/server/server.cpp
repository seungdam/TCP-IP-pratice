#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

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

int main(int argc, char* argv) {
	WSADATA wsa;
	int retval;

	if (WSAStartup(MAKEWORD(3, 2), &wsa) != 0) return 1;

	// 클라이언트와 연결하기 위한 소켓
	SOCKET listen_sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");
	
	//bind()
	sockaddr_in6 serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin6_family = AF_INET6;
	serveraddr.sin6_port = htons(SERVERPORT);
	serveraddr.sin6_addr = in6addr_any; // serveraddr.sin_addr.s_addr = IN_ADDRANY;
	retval = bind(listen_sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// tcp 상태를 listen으로 설정
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen");

	// 클라이언트와 통신하기 위한 소켓 accept() 를 사용해 생성한다.

	SOCKET client_sock;
	sockaddr_in6 clientaddr;
	int addrlen;
	
	
	// 전송할 메세지 버퍼 크기
	char buf[BUFSIZE + 1];
	
	while (true) {
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		char ipaddr[50];
		DWORD ipaddrlen = sizeof(ipaddr);
		WSAAddressToString((sockaddr*)&clientaddr, sizeof(clientaddr), NULL, ipaddr,&ipaddrlen);
		cout << "[TCP 서버]" << " 클라이언트 접속 : " << ipaddr << endl;


		// 데이터 수신
		while (true) {
			retval = recv(client_sock, buf, BUFSIZE + 1, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) // 보낸 데이터가 0일 경우
				break;

			buf[retval] =  '\0';
			cout << "[TCP\\" << ipaddr << "] : " << buf << endl;

			// 클라이언트에게 다시 재전송
			retval = send(client_sock, buf, retval, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break; 
			}	
		}
		closesocket(client_sock);
	}
	closesocket(listen_sock);
	WSACleanup();
}