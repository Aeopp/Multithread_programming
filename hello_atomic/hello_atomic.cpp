#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// 그러나 이상황에서 서로 같은 캐쉬라인을 쓰레드들이 사용하고 있기 때문에
// 쓰레드 별로 counter 의 값을 바꿀때마다 false sharing 문제가 발생한다.
void worker(std::atomic<int>& counter)
{
	for (int i = 0; i < 10000; ++i)
	{
		counter++;
	}
}

int main() {
	std::atomic<int> counter(0);

	std::vector<std::thread> workers;

	for (int i = 0; i < 4; ++i)
	{
		workers.push_back(std::thread(worker, std::ref(counter)));
	}

	for (int i = 0; i < 4; ++i)
	{
		workers[i].join();
	}

	std::cout << "Counter 최종 값 : " << counter << std::endl;
}