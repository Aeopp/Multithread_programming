#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
using std::memory_order_relaxed;

void producer(std::atomic<bool>* is_ready, int* data)
{
	// data 에 값을 쓰기도 전에 ready 가 true 가 될수 있다.
	*data = 10;
	is_ready->store(true,std::memory_order::memory_order_release);
}

void consumer(std::atomic<bool>* is_ready, int* data)
{
	// data 가 채 준비되기도 전에 data 가 출력될수 있다.
	while (!is_ready->load(std::memory_order_acquire));

	std::cout << "Data : " << *data << std::endl; 
}

int main()
{
	std::vector<std::thread> threads;
	std::atomic<bool > is_ready(false);

	int data = 0;
	threads.push_back(std::thread(producer, &is_ready, &data));
	threads.push_back(std::thread(consumer, &is_ready, &data));

	for (int i = 0; i < 2; ++i)
	{
		threads[i].join();
	}
}