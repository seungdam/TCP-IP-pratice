#include <process.h>
#include <windows.h>
#include <iostream>
#define MAXNUM 100000

using namespace std;
int num = 0;

unsigned __stdcall MyThread(LPVOID arg) {
	for (int i = 0; i < MAXNUM; ++i)
		++num;
	return 0;
}

unsigned __stdcall MyThread2(LPVOID arg) {
	for (int i = 0; i < MAXNUM; ++i)
		--num;
	return 0;
}

int main() {
	
	HANDLE m_thread[2];
	m_thread[0] = (HANDLE)_beginthreadex(NULL, 0, MyThread, NULL, 0, NULL);
	m_thread[1] = (HANDLE)_beginthreadex(NULL, 0, MyThread2, NULL, 0, NULL);

	WaitForMultipleObjects(2, m_thread, TRUE, INFINITE);
	cout << num << endl;
	// ���� �ڿ��� ���δٸ� �����忡�� ���ÿ� �����ϱ⿡ �������� ��°��� ������ �ȴ�.

}

