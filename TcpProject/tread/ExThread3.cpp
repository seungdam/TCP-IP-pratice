#include <process.h>
#include <windows.h>
#include <iostream>

int sum = 0;

unsigned __stdcall MyThread(LPVOID arg) {
	int num = (int)arg;
	
	for (int i = 0; i <= num; ++i) {
		sum += i;
		Sleep(10);
	}
	return 0;
}

int main() {

	int retval = 0;
	int arg = 1000;
	HANDLE m_thread = (HANDLE)_beginthreadex(NULL, 0, MyThread, (LPVOID)arg, CREATE_SUSPENDED, NULL);
	if (m_thread == NULL) return 1;
	std::cout << "실행 전:" << sum << std::endl;

	// 서스펜드 상태로 생성했으니 바로 수행하지 않음. -> resumethread 호출해야함.
	ResumeThread(m_thread);
	// 스레드가 기능을 모두 수행할 때 까지 기다림.
	retval = WaitForSingleObject(m_thread, 5000);
	if (retval == WAIT_TIMEOUT) {
		std::cout << "*" << std::endl;
		WaitForSingleObject(m_thread, INFINITE);
	}

	std::cout << "실행 후: " << sum << std::endl;
	CloseHandle(m_thread);
}