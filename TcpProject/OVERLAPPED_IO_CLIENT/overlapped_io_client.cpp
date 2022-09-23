#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 9000
#define BUFSIZE 256

// 오류 결과 출력을 위한 함수들
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
	//std::cout << "[" << msg << "] : " << (char*)lpMsgBuf << std::endl;
	printf("[%s] : %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

SOCKET s_sock; // 서버 소켓
WSAOVERLAPPED s_over; // overlapped 구조체
WSABUF s_wsabuf[1];
char s_msg[BUFSIZE];

// OS에서 호출할 callback 함수 선언
void CALLBACK recv_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags);
void CALLBACK send_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags);

// 송수신 시작 함수
void do_send();

// Overlapped I/O CALLBACK 모델
int main() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

	
	sockaddr_in serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
	InetPton(AF_INET, SERVER_ADDR, &serveraddr.sin_addr.s_addr);
	
	while (1) SleepEx(100, true);
	closesocket(s_sock);
	WSACleanup();

}

void do_send() {
	printf("chat: ");
	std::cin.getline(s_msg, BUFSIZE);
	printf("\n");

	s_wsabuf[0].len = strlen(s_msg);
	s_wsabuf[0].buf = s_msg;
	ZeroMemory(&s_over, sizeof(s_over));
	WSASend(s_sock, s_wsabuf, 1, 0, 0, &s_over, send_callback);
	
	do_send();
}

// WSARecv 완료 시 OS에서 자동으로 호출
void CALLBACK recv_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags) {
	if (0 == num_bytes) return;
	printf("Send from Server: %s", s_msg);
	do_send();
}

void CALLBACK send_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags) {
	s_wsabuf[0].len = BUFSIZE;
	DWORD r_flag = 0;
	ZeroMemory(&s_over, 0);
	WSARecv(s_sock, s_wsabuf, 1, 0, &r_flag, &s_over, recv_callback);
}