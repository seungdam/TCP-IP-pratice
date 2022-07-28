#include <process.h>
#include <windows.h>
#include <iostream>

int retval = 0;

unsigned __stdcall MyThread(LPVOID arg) {
	int num = (int)arg;
	
	for (int i = 0; i <= num; ++i) {
		retval += i;
	}
	return 0;
}

int main() {

	int arg = 1000;
	HANDLE m_thread = (HANDLE)_beginthreadex(NULL, 0, MyThread, (LPVOID)arg, CREATE_SUSPENDED, NULL);
	if (m_thread == NULL) return 1;
	std::cout << "���� ��:" << retval << std::endl;

	// ������� ���·� ���������� �ٷ� �������� ����. -> resumethread ȣ���ؾ���.
	ResumeThread(m_thread);
	// �����尡 ����� ��� ������ �� ���� ��ٸ�.
	WaitForSingleObject(m_thread, INFINITE);
	std::cout << "���� ��: " << retval << std::endl;
	CloseHandle(m_thread);
}