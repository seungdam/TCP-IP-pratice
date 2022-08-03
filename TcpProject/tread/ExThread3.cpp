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
	std::cout << "���� ��:" << sum << std::endl;

	// ������� ���·� ���������� �ٷ� �������� ����. -> resumethread ȣ���ؾ���.
	ResumeThread(m_thread);
	// �����尡 ����� ��� ������ �� ���� ��ٸ�.
	retval = WaitForSingleObject(m_thread, 5000);
	if (retval == WAIT_TIMEOUT) {
		std::cout << "*" << std::endl;
		WaitForSingleObject(m_thread, INFINITE);
	}

	std::cout << "���� ��: " << sum << std::endl;
	CloseHandle(m_thread);
}