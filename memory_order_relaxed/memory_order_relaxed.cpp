#include <atomic>
#include <cstdio>
#include <thread>
#include <vector>
#include <iostream>

using std::memory_order_relaxed;

void t1(std::atomic<int>* a, std::atomic<int>* b)
{
	b->store(1, memory_order_relaxed);
	int x = a->load(memory_order_relaxed);

	std::cout << 'a';
	std::cout << 'b';
	std::cout << 'c';
	std::cout << 'd';
	std::cout << 'e';
	std::cout << 'f';

	printf("x : %d \n", x);
}

void t2(std::atomic<int>* a, std::atomic<int>* b)
{
	a->store(1, memory_order_relaxed);
	int y = b->load(memory_order_relaxed);
	std::cout << 1;
	std::cout << 2;
	std::cout << 3;
	std::cout << 4;
	std::cout << 5;
	std::cout << 6;

	printf("y : %d \n", y);
}

int main() {
	std::vector<std::thread> threads;

	std::atomic<int> a(0);
	std::atomic<int> b(0);

	threads.push_back(std::thread(t1, &a, &b));
	threads.push_back(std::thread(t2, &a, &b));

	for (int i = 0; i < 2; ++i)
	{
		threads[i].join();
	}
}