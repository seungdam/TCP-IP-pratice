#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <unordered_map>

#define SERVER_PORT 9000
#define BUFSIZE 256

// ���� ��� ����� ���� �Լ���
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

class SESSION {
	SOCKET sock; // Ŭ���̾�Ʈ ����
	WSAOVERLAPPED recv_over; // overlapped ����ü
	WSABUF recv_wsabuf;
	WSABUF send_wsabuf;
	char recv_msg[BUFSIZE];
	char send_msg[BUFSIZE];
	int id; // �ʿ� ������ session���� Ž���ϱ� ����
public:
	SESSION() { printf("error"); exit(1); };
	SESSION(int id,SOCKET s):id(id),sock(s) {
		recv_wsabuf.len = BUFSIZE;
		recv_wsabuf.buf = recv_msg;
		send_wsabuf.len = 0;
		send_wsabuf.buf = send_msg;
	}
	void do_send(int num_bytes) {
		ZeroMemory(&recv_over, sizeof(recv_over));
		recv_over.hEvent = (HANDLE)id; // �ٸ� Ŭ���̾�Ʈ���� ���۵� �� ���° Ŭ�󿡼� ���۵� ���������� �˷��ֱ� ����.
		send_wsabuf.len = num_bytes;
		WSASend(sock, &send_wsabuf, 1, 0, 0, &recv_over, send_callback);
	}
	void do_recv() {
		ZeroMemory(&recv_over, sizeof(recv_over));
		recv_over.hEvent = (HANDLE)id; // �ش� �����Ͱ� � Ŭ���̾�Ʈ�� ���� ���۵ƴ��� �����ϱ� ����.
		DWORD recv_flag = 0;
		WSARecv(sock, &recv_wsabuf, 1, 0, &recv_flag, &recv_over, recv_callback);
	}
	~SESSION();
};

std::unordered_map<SOCKET, SESSION> clients;

// OS���� ȣ���� callback �Լ� ����
void CALLBACK recv_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags);
void CALLBACK send_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags);

// �ۼ��� ���� �Լ�
void do_recv();

// Overlapped I/O CALLBACK ��
int main() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

	SOCKET s_sock = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED); // overlapped �����ϴ� ���� ����
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
	c_sock = WSAAccept(s_sock, (sockaddr*)&clientaddr, &addrlen, NULL, NULL);
	
	// ���μ����� �񵿱� ������ ���� �� �ִ� alertable ���·� ����
	do_recv();
	while (1) SleepEx(100,true);

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

// WSARecv �Ϸ� �� OS���� �ڵ����� ȣ��
void CALLBACK recv_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags) {
	if (0 == num_bytes) return;
	printf("Send from Client : %s", c_msg);
	c_wsabuf[0].len = num_bytes;
	ZeroMemory(&c_over, sizeof(c_over));
	WSASend(c_sock, c_wsabuf, 1, 0, 0, &c_over, send_callback);
}

void CALLBACK send_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED over, DWORD flags) {
	do_recv();
}