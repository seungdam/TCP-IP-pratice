#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <unordered_map>

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

// OS에서 호출할 callback 함수 선언
void CALLBACK recv_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags);
void CALLBACK send_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags);

class SESSION {
	SOCKET sock; // 클라이언트 소켓
	WSAOVERLAPPED recv_over; // overlapped 구조체
	WSABUF recv_wsabuf;
	WSABUF send_wsabuf;
	int id; // 맵에 저장한 session들을 탐색하기 위함
public:
	char recv_msg[BUFSIZE];
	SESSION() { printf("error"); exit(1); };
	SESSION(int id,SOCKET s):id(id),sock(s) {
		recv_wsabuf.len = BUFSIZE;
		recv_wsabuf.buf = recv_msg;
		send_wsabuf.len = 0;
		send_wsabuf.buf = recv_msg;
	}
	void do_send(int num_bytes) {
		ZeroMemory(&recv_over, sizeof(recv_over));
		recv_over.hEvent = (HANDLE)id; // 다른 클라이언트에게 전송될 때 몇번째 클라에서 전송된 데이터인지 알려주기 위함.
		send_wsabuf.len = num_bytes;
		WSASend(sock, &send_wsabuf, 1, 0, 0, &recv_over, send_callback);
	}
	void do_recv() {
		ZeroMemory(&recv_over, sizeof(recv_over));
		recv_over.hEvent = (HANDLE)id; // 해당 데이터가 어떤 클라이언트로 부터 전송됐는지 구별하기 위함.
		DWORD recv_flag = 0;
		WSARecv(sock, &recv_wsabuf, 1, 0, &recv_flag, &recv_over, recv_callback);
	}
	~SESSION() { closesocket(sock); };
};

std::unordered_map<int, SESSION> clients;



// 송수신 시작 함수
void do_recv();

// Overlapped I/O CALLBACK 모델
int main() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

	SOCKET s_sock = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED); // overlapped 지원하는 소켓 생성
	if (INVALID_SOCKET == s_sock) err_quit("WSAsocket()");
	
	sockaddr_in serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);

	int retval;
	retval = bind(s_sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == retval) err_quit("bind()");

	retval = listen(s_sock, SOMAXCONN);
	if (SOCKET_ERROR == retval) err_quit("listen()");

	sockaddr_in clientaddr;
	int addrlen = sizeof(clientaddr);
	//c_sock = WSAAccept(s_sock, (sockaddr*)&clientaddr, &addrlen, NULL, NULL);
	//
	//// 프로세스를 비동기 통지를 받을 수 있는 alertable 상태로 변경
	//do_recv();
	//while (1) SleepEx(100,true);
	for (int i = 1; ; ++i) {
		SOCKET c_socket = WSAAccept(s_sock, (sockaddr*)(&clientaddr),&addrlen, 0, 0);
		clients.try_emplace(i, i, c_socket);
		printf("[%d] 클라이언트 연결\n", i);
		clients[i].do_recv();
	}
	clients.clear();
	closesocket(s_sock);
	WSACleanup();
	
}

//void do_recv() {
//	c_wsabuf[0].len = BUFSIZE;
//	c_wsabuf[0].buf = c_msg;
//	DWORD recv_flag = 0;
//	ZeroMemory(&c_over, sizeof(c_over));
//	WSARecv(c_sock, c_wsabuf, 1, NULL, &recv_flag, &c_over, recv_callback);
//}

// WSARecv 완료 시 OS에서 자동으로 호출
void CALLBACK recv_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags) {
	int s_id = (int)over->hEvent;
	printf("[%d Client] : %s \n",s_id,clients[s_id].recv_msg);
	clients[s_id].do_send(num_bytes);
}

void CALLBACK send_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags) {
	int s_id = (int)over->hEvent;
	clients[s_id].do_recv();
}