#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#define SERVERPORT 9000
#define BUFSIZE    512

using std::cout;
using std::endl;
// ���� �Լ� ���� ��� �� ����
void err_quit(char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

// ���� �Լ� ���� ���
void err_display(char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    cout << "[" << msg << "]" <<  (char*)lpMsgBuf;
    LocalFree(lpMsgBuf);
}

// TCP ����(IPv4)
DWORD WINAPI TCPServer4(LPVOID arg)
{
    int retval;

    // socket()
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    // ������ ��ſ� ����� ����
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        // ������ Ŭ���̾�Ʈ ���� ���
        cout << endl;
        cout << "\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = " << inet_ntoa(clientaddr.sin_addr)
            << " ��Ʈ ��ȣ = " << ntohs(clientaddr.sin_port);

        // Ŭ���̾�Ʈ�� ������ ���
        while (1) {
            // ������ �ޱ�
            retval = recv(client_sock, buf, BUFSIZE, 0);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
                break;

            // ���� ������ ���
            buf[retval] = '\0';
            cout << buf;
        }

        // closesocket()
        closesocket(client_sock);
        cout << "[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ� = " << inet_ntoa(clientaddr.sin_addr) 
            << " ��Ʈ ��ȣ = " << ntohs(clientaddr.sin_port);
    }

    // closesocket()
    closesocket(listen_sock);

    return 0;
}

// TCP ����(IPv6)
DWORD WINAPI TCPServer6(LPVOID arg)
{
    int retval;

    // socket()
    SOCKET listen_sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    SOCKADDR_IN6 serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin6_family = AF_INET6;
    serveraddr.sin6_addr = in6addr_any;
    serveraddr.sin6_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    // ������ ��ſ� ����� ����
    SOCKET client_sock;
    SOCKADDR_IN6 clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        // ������ Ŭ���̾�Ʈ ���� ���
        char ipaddr[50];
        DWORD ipaddrlen = sizeof(ipaddr);
        WSAAddressToString((SOCKADDR*)&clientaddr, sizeof(clientaddr),
            NULL, ipaddr, &ipaddrlen);
        printf("\n[TCP ����] Ŭ���̾�Ʈ ����: %s\n", ipaddr);

        // Ŭ���̾�Ʈ�� ������ ���
        while (1) {
            // ������ �ޱ�
            retval = recv(client_sock, buf, BUFSIZE, 0);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
                break;

            // ���� ������ ���
            buf[retval] = '\0';
            printf("%s", buf);
        }

        // closesocket()
        closesocket(client_sock);
        cout << "[TCP ����] Ŭ���̾�Ʈ ����: " << ipaddr << endl;
    }

    // closesocket()
    closesocket(listen_sock);

    return 0;
}

// ����� ���� ���� �Լ� 
int f(int x) {
    if (x >= 0) { 
        WSASetLastError(0);
        return 0; 
    }
    else {
        int errorcode = WSAEMSGSIZE;
        WSASetLastError(errorcode);
        return SOCKET_ERROR;
    }
}

int main(int argc, char* argv[])
{

    // p.61 �������� 1
    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(3, 2), &wsa) != 0)
        return 1;

    cout << wsa.wVersion << endl; 
    cout << wsa.wHighVersion << endl;
    cout << wsa.szDescription << endl;
    cout << wsa.szSystemStatus << endl;
    MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);
    
    // �������� 2
    // udp ���� ���� IPv6 �ּ�ü�� , �񿬰Ἲ(udp) udp ��������;
    SOCKET udp_socket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    
    // ���� ���� ���н� ���� ���
    if (udp_socket == INVALID_SOCKET) err_quit("socket()");
    MessageBox(NULL, "udp ���� ���� ����", "�˸�", MB_OK);
    closesocket(udp_socket);

    // �������� 3
    SOCKET tcp_socket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (tcp_socket == INVALID_SOCKET) err_quit("socket()");
    MessageBox(NULL, "tcp ���� ���� ����", "�˸�", MB_OK);
    closesocket(tcp_socket);

    // �������� 4
    SOCKET tmp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP); // ���� �߻� ����
    int retval = f(tmp_socket);

    if (retval == SOCKET_ERROR)
        err_quit("socket()");
    closesocket(tmp_socket);
    // ���� ����

    // �������� 5
    SOCKET wsa_tcp_socket = WSASocket(AF_INET6, SOCK_STREAM, IPPROTO_TCP, NULL,0, WSA_FLAG_OVERLAPPED);
    if (wsa_tcp_socket == INVALID_SOCKET) err_quit("socket()");
    closesocket(wsa_tcp_socket);
    
    WSACleanup();
    return 0;
}
