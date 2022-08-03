#include <windows.h>
#include <process.h>
#include <iostream>

using std::cout;
using std::endl;

struct POSITION {
	int x, y, z;
};

DWORD WINAPI f(LPVOID arg) {
	int a = (int)arg;
	while(true) {
		cout << "Thread ID: " << GetCurrentThreadId()<< endl;
		Sleep(1500);
	}
}
// _beginthreadex를 위한 스레드 함수 형태
unsigned int __stdcall f2(LPVOID arg) {
	POSITION* ptr = (POSITION*)arg;

	while (true) {
		cout << "Thread ID: " << GetCurrentThreadId() << 
			" Position: " << ptr->x << " " << ptr->y << " " << ptr->z << endl;
		Sleep(1000);
	}
}

int main() {
	//HANDLE m_thread1 = CreateThread(NULL, 0, f, NULL, 0, NULL);

	POSITION ps1{ 10,20,30 };
	POSITION ps2{ 40,50,60 };
	
	// 1. 보안 변수 2. 초기 스택 사이즈 3. 스레드 함수 모듈 4. 생성 옵션 5. 스레드 id
	HANDLE m_thread1 = (HANDLE)_beginthreadex(NULL, 0, f2, (LPVOID)&ps1, 0, NULL);
	if (m_thread1 == NULL) return 1;
	CloseHandle(m_thread1);
	//HANDLE m_thread2 = CreateThread(NULL, 0, f, NULL, 0, NULL);
	HANDLE m_thread2 = (HANDLE)_beginthreadex(NULL, 0, f2, (LPVOID)&ps2, 0, NULL);
	if (m_thread2 == NULL) return 1;
	CloseHandle(m_thread2); // 스레드를 조작할 일이 없으면 가능한 일찍 닫는 것이 중요. != 스레드 종료
	
	while (true) {
		cout << "Main Thread ID: " << GetCurrentThreadId() << endl;
		Sleep(1000);
	}
}