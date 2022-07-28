#include <process.h>
#include <windows.h>
#include <iostream>
#define MAXNUM 200000

using namespace std;
int num = 0;

CRITICAL_SECTION cs; // 임계 영역 구조체 변수 cs 생성

unsigned __stdcall MyThread(LPVOID arg) {
	
	for (int i = 0; i < MAXNUM; ++i) {
		EnterCriticalSection(&cs);
		++num;
		LeaveCriticalSection(&cs);
	}
	return 0;
}

unsigned __stdcall MyThread2(LPVOID arg) {
	for (int i = 0; i < MAXNUM; ++i) {
		EnterCriticalSection(&cs);
		--num;
		LeaveCriticalSection(&cs);
	}
	return 0;
}

int main() {
	// 임계 영역 초기화
	InitializeCriticalSection(&cs);

	HANDLE m_thread[2];
	m_thread[0] = (HANDLE)_beginthreadex(NULL, 0, MyThread, NULL, 0, NULL);
	m_thread[1] = (HANDLE)_beginthreadex(NULL, 0, MyThread2, NULL, 0, NULL);

	WaitForMultipleObjects(2, m_thread, TRUE, INFINITE);
	cout << num << endl;
	
	// 임계 영역 삭제
	DeleteCriticalSection(&cs);
}

