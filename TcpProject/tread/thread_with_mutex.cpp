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
	// 뮤텍스를 하나도 안사용했을 때의 성능은 왜 뮤텍스를 사용했을 때 보다 느린가? --> 캐시 line 단위의 캐시 핑퐁 떄문
	// 필요한 부분만 가져오는 것이 아닌 한 뭉텅이로 가져온다 (캐시 적중률을 높이기 위함)
	// 하지만 이렇게 하면 안됨.. --> 언젠가는 메모리의 업데이트가 끝날 텐데 이때 메모리에서 업데이트한 작업을 복사하는 과정에서 오버헤드가 발생.
	// 모든 코어가 동일한 캐시라인을 접근하기 떄문에.. 서로 확인하는 과정을 거친다..

	// 현재는 cpu가 발전해 각 캐시간의 고속 통로가 존재한다. 그 덕분에 성능이 완전히 느려지는 것은 면했지만.. 뮤텍스를 사용하는 것에 비할바가 아니다.
	// 운영체제마다 캐시라인 값을 알아내는 api가 존재
	// *16을 통해 캐시 라인만큼 데이터를 읽게한다.
	// * 16대신 allignas(64)을 사용해보자.
}