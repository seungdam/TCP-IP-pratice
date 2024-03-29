#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#include <iostream>

#define SERVERPORT 4444
#define BUFSIZE 50

using namespace std;

struct SOCKANDADDR {
	SOCKET s;
	sockaddr_in addr;
};


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

unsigned __stdcall ProcessClient(LPVOID arg) {
	
	int retval;
	
	SOCKANDADDR* ms = (SOCKANDADDR*)arg;
	int addrlen;
	char buf[BUFSIZE + 1];

	addrlen = sizeof(ms->addr);

	while (true) {
		retval = recvn(ms->s, buf, BUFSIZE, 0);
		
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) break;

		buf[retval] = '\0';
		cout << "[TCP/" << inet_ntoa(ms->addr.sin_addr) << ":" << ntohs(ms->addr.sin_port) << "] 전송된 데이터: " << buf << endl;
		retval = send(ms->s, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) break;
	}
	closesocket(ms->s);
	_endthreadex(0);
	return 0;
}

int main() {
	//현재 스레드의 우선순위를 변경
	SetThreadPriority(GetCurrentProcess(), THREAD_PRIORITY_ABOVE_NORMAL);
	GetCurrentProcess();
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(3, 2), &wsa) != 0) return 1;
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_sock == INVALID_SOCKET)  err_quit("socket()");

	sockaddr_in serveraddr;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVERPORT);
	// bind
	retval = bind(listen_sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	SOCKET client_sock;
	sockaddr_in clientaddr;
	int addrlen;

	while (true) {
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
		cout << "[TCP 서버]" << " 클라이언트 접속 : IP 주소: " << inet_ntoa(clientaddr.sin_addr) << "포트 번호: " << ntohs(clientaddr.sin_port) << endl;
		
		SOCKANDADDR ms;
		ms.s = client_sock;
		ms.addr = clientaddr;

		HANDLE m_thread = (HANDLE)_beginthreadex(NULL, 0, ProcessClient, (LPVOID)&ms, 0, NULL);
		if (m_thread == NULL) closesocket(client_sock);
		else CloseHandle(m_thread);
	}
	closesocket(listen_sock);
	WSACleanup();
	return 0;
}