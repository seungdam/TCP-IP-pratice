#include <process.h>
#include <windows.h>
#include <iostream>

unsigned int __stdcall MyThread(LPVOID arg) {
	while (true);
	return 0;
}

int main() {
	// cpu ������ �˾Ƴ��� ����
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	for (int i = 0; i < si.dwNumberOfProcessors - 1; ++i) {
		HANDLE m_thread = (HANDLE)_beginthreadex(NULL, 0, MyThread, 0, 0, NULL);
		if (m_thread == NULL) return 1;
		SetThreadPriority(m_thread, THREAD_PRIORITY_TIME_CRITICAL); // �켱������ ���� ���� ����
		CloseHandle(m_thread);
	}

	std::cout << "main thread" << std::endl;
}