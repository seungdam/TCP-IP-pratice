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
	CloseHandle(m_thread2); // �����带 ������ ���� ������ ������ ���� �ݴ� ���� �߿�. != ������ ����

	while (true) {
		cout << "Main Thread ID: " << GetCurrentThreadId() << endl;
		Sleep(1000);
	}
}