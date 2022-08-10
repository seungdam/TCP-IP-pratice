#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#define REMOTEIP "255.255.255.255"
#define REMOTEPORT 9000
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
	
	// ��ε� ĳ���� Ȱ��ȭ
	bool bEnable = true;
	retval = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&bEnable, sizeof(bEnable));
	if (retval == SOCKET_ERROR) err_quit("setsockopt()");

	// ��ε�ĳ��Ʈ ip �ּ� ����ü �ʱ�ȭ
	sockaddr_in remoteaddr;
	ZeroMemory(&remoteaddr, sizeof(remoteaddr));
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_addr.s_addr = inet_addr(REMOTEIP);
	remoteaddr.sin_port = htons(REMOTEPORT);

	// ������ �۽�
	char buf[BUFSIZE + 1];

	while (1) {
		cout << "chat: ";
		cin.getline(buf, BUFSIZE);
		retval = sendto(sock, buf, strlen(buf), 0, (sockaddr*)&remoteaddr, sizeof(remoteaddr));
		if (retval == SOCKET_ERROR) {
			err_display("sendto()");
			continue;
		}
		cout << "\n" << retval << "����Ʈ ���� �Ϸ�" << endl;
	}

	closesocket(sock);
	WSACleanup();
	
}