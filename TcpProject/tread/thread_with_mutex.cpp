#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

const auto MAX_THREADS = 16;
using namespace std;
using namespace std::chrono;
//volatile int sum[MAX_THREADS * 16];
// mutex sum_lock;

// allignas(64)

struct NUM {
	alignas(64) volatile int num;
};
NUM sum[MAX_THREADS];

void thread_func(int num_threads, int th_id) {
	// volatile int local_sum = 0;
	for (auto i = 0; i < 50000000 / num_threads; ++i) {
		sum[th_id].num = sum[th_id].num;
		//local_sum += 2;
	}

	//sum_lock.lock();
	//sum += local_sum;
	//sum_lock.unlock();

}
int main() {
	
	//vector<thread> threads;
	for (auto i = 1; i <= MAX_THREADS; i *= 2) {
		//sum = 0;
		vector<thread> threads;
		for (auto& i : sum) i.num = 0;
		auto start = high_resolution_clock::now();
		for (auto j = 0; j < i; ++j) threads.emplace_back(thread_func, i,i);
		for (auto& tmp : threads) tmp.join();
		auto duration = high_resolution_clock::now() - start;
		cout << i << " Threads, Sum = " << sum;
		cout << " Duration = " << duration_cast<milliseconds>(duration).count() << " ms\n";
	}
	// ���ؽ��� �ϳ��� �Ȼ������ ���� ������ �� ���ؽ��� ������� �� ���� ������? --> ĳ�� line ������ ĳ�� ���� ����
	// �ʿ��� �κи� �������� ���� �ƴ� �� �����̷� �����´� (ĳ�� ���߷��� ���̱� ����)
	// ������ �̷��� �ϸ� �ȵ�.. --> �������� �޸��� ������Ʈ�� ���� �ٵ� �̶� �޸𸮿��� ������Ʈ�� �۾��� �����ϴ� �������� ������尡 �߻�.
	// ��� �ھ ������ ĳ�ö����� �����ϱ� ������.. ���� Ȯ���ϴ� ������ ��ģ��..

	// ����� cpu�� ������ �� ĳ�ð��� ��� ��ΰ� �����Ѵ�. �� ���п� ������ ������ �������� ���� ��������.. ���ؽ��� ����ϴ� �Ϳ� ���ҹٰ� �ƴϴ�.
	// �ü������ ĳ�ö��� ���� �˾Ƴ��� api�� ����
	// *16�� ���� ĳ�� ���θ�ŭ �����͸� �а��Ѵ�.
	// * 16��� allignas(64)�� ����غ���.
}