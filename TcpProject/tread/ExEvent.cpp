#include <windows.h>
#include <process.h>
#include <iostream>

#define BUFSIZE 10
HANDLE m_readEvent;
HANDLE m_writeEvent;
int buf[BUFSIZE];
unsigned __stdcall WriteThread(LPVOID) {
	int retval;
	for (int i = 0; i <= 500; ++i) {
		retval = WaitForSingleObject(m_readEvent, INFINITE); // 파일 읽기가 모두 끝날 때까지 대기
		if (retval != WAIT_OBJECT_0) break; //  읽기 스레드가 정상적으로 종료되지 않았다면 루프를 벗어남.

		for (int k = 0; k < BUFSIZE; ++k) {
			buf[k] = i;
		}

		SetEvent(m_writeEvent); // 쓰기 완료. 비신호 -> 신호
	}
	return 0;
}
unsigned __stdcall ReadThread(LPVOID) {
	int retval;

	while (true) {
		retval = WaitForSingleObject(m_writeEvent, INFINITE);
		if (retval != WAIT_OBJECT_0) break;

		std::cout << "ThreadID: " << GetCurrentThreadId() << " ";
		for (auto i : buf)
			std::cout << i << " ";
		std::cout << std::endl;

		ZeroMemory(buf, BUFSIZE);
		SetEvent(m_readEvent);
	}
	return 0;
}


int main() {

	m_writeEvent = CreateEvent(NULL, FALSE, FALSE, NULL); // 비신호 상태
	if (m_writeEvent == NULL) return 1;
	m_readEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (m_readEvent == NULL) return 1;

	HANDLE handles[3];
	handles[0] = (HANDLE)_beginthreadex(NULL,0,  WriteThread,NULL,0,NULL);
	handles[1] = (HANDLE)_beginthreadex(NULL, 0, ReadThread, NULL, 0, NULL);
	handles[2] = (HANDLE)_beginthreadex(NULL, 0, ReadThread, NULL, 0, NULL);

	WaitForMultipleObjects(3, handles, TRUE, INFINITE);

	CloseHandle(m_writeEvent);
	CloseHandle(m_readEvent);
	return 0;
}