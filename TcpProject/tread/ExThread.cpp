#include <windows.h>
#include <iostream>

using std::cout;
using std::endl;

DWORD WINAPI f(LPVOID arg) {
	int a = (int)arg;
	while(true) {
		cout << "Thread ID: " << GetCurrentThreadId()<< endl;
		Sleep(1500);
	}
}

int main() {
	HANDLE m_thread1 = CreateThread(NULL, 0, f, NULL, 0, NULL);

	if (m_thread1 == NULL) return 1;
	CloseHandle(m_thread1);

	HANDLE m_thread2 = CreateThread(NULL, 0, f, NULL, 0, NULL);
	if (m_thread2 == NULL) return 1;
	CloseHandle(m_thread2); // 스레드를 조작할 일이 없으면 가능한 일찍 닫는 것이 중요. != 스레드 종료

	while (true) {
		cout << "Main Thread ID: " << GetCurrentThreadId() << endl;
		Sleep(1000);
	}
}