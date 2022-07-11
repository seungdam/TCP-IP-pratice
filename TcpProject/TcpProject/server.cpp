#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#define SERVERPORT 9000
#define BUFSIZE    512

using std::cout;
using std::endl;
// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

// TCP 서버(IPv4)
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

    // 데이터 통신에 사용할 변수
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

        // 접속한 클라이언트 정보 출력
        cout << endl;
        cout << "\n[TCP 서버] 클라이언트 접속 : IP 주소 = " << inet_ntoa(clientaddr.sin_addr)
            << " 포트 번호 = " << ntohs(clientaddr.sin_port);

        // 클라이언트와 데이터 통신
        while (1) {
            // 데이터 받기
            retval = recv(client_sock, buf, BUFSIZE, 0);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
                break;

            // 받은 데이터 출력
            buf[retval] = '\0';
            cout << buf;
        }

        // closesocket()
        closesocket(client_sock);
        cout << "[TCP 서버] 클라이언트 종료: IP 주소 = " << inet_ntoa(clientaddr.sin_addr) 
            << " 포트 번호 = " << ntohs(clientaddr.sin_port);
    }

    // closesocket()
    closesocket(listen_sock);

    return 0;
}

// TCP 서버(IPv6)
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

    // 데이터 통신에 사용할 변수
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

        // 접속한 클라이언트 정보 출력
        char ipaddr[50];
        DWORD ipaddrlen = sizeof(ipaddr);
        WSAAddressToString((SOCKADDR*)&clientaddr, sizeof(clientaddr),
            NULL, ipaddr, &ipaddrlen);
        printf("\n[TCP 서버] 클라이언트 접속: %s\n", ipaddr);

        // 클라이언트와 데이터 통신
        while (1) {
            // 데이터 받기
            retval = recv(client_sock, buf, BUFSIZE, 0);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
                break;

            // 받은 데이터 출력
            buf[retval] = '\0';
            printf("%s", buf);
        }

        // closesocket()
        closesocket(client_sock);
        cout << "[TCP 서버] 클라이언트 종료: " << ipaddr << endl;
    }

    // closesocket()
    closesocket(listen_sock);

    return 0;
}

// 사용자 정의 에러 함수 
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

    // p.61 연습문제 1
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(3, 2), &wsa) != 0)
        return 1;

    cout << wsa.wVersion << endl; 
    cout << wsa.wHighVersion << endl;
    cout << wsa.szDescription << endl;
    cout << wsa.szSystemStatus << endl;
    MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);
    
    // 연습문제 2
    // udp 소켓 생성 IPv6 주소체계 , 비연결성(udp) udp 프로토콜;
    SOCKET udp_socket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    
    // 소켓 생성 실패시 에러 출력
    if (udp_socket == INVALID_SOCKET) err_quit("socket()");
    MessageBox(NULL, "udp 소켓 생성 성공", "알림", MB_OK);
    closesocket(udp_socket);

    // 연습문제 3
    SOCKET tcp_socket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (tcp_socket == INVALID_SOCKET) err_quit("socket()");
    MessageBox(NULL, "tcp 소켓 생성 성공", "알림", MB_OK);
    closesocket(tcp_socket);

    // 연습문제 4
    SOCKET tmp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP); // 에러 발생 소켓
    int retval = f(tmp_socket);

    if (retval == SOCKET_ERROR)
        err_quit("socket()");
    closesocket(tmp_socket);
    // 윈속 종료

    // 연습문제 5
    SOCKET wsa_tcp_socket = WSASocket(AF_INET6, SOCK_STREAM, IPPROTO_TCP, NULL,0, WSA_FLAG_OVERLAPPED);
    if (wsa_tcp_socket == INVALID_SOCKET) err_quit("socket()");
    closesocket(wsa_tcp_socket);
    
    WSACleanup();
    return 0;
}
