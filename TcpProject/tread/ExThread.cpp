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
// _beginthreadex�� ���� ������ �Լ� ����
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
	
	// 1. ���� ���� 2. �ʱ� ���� ������ 3. ������ �Լ� ��� 4. ���� �ɼ� 5. ������ id
	HANDLE m_thread1 = (HANDLE)_beginthreadex(NULL, 0, f2, (LPVOID)&ps1, 0, NULL);
	if (m_thread1 == NULL) return 1;
	CloseHandle(m_thread1);
	//HANDLE m_thread2 = CreateThread(NULL, 0, f, NULL, 0, NULL);
	HANDLE m_thread2 = (HANDLE)_beginthreadex(NULL, 0, f2, (LPVOID)&ps2, 0, NULL);
	if (m_thread2 == NULL) return 1;
	CloseHandle(m_thread2); // �����带 ������ ���� ������ ������ ���� �ݴ� ���� �߿�. != ������ ����
	
	while (true) {
		cout << "Main Thread ID: " << GetCurrentThreadId() << endl;
		Sleep(1000);
	}
}