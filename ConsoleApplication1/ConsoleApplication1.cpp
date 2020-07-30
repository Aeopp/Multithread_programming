#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

void worker1(std::mutex& m1, std::mutex& m2)
{
	for (int i = 0; i < 10; ++i)
	{
		m1.lock();
		m2.lock();
		std::cout << "Worker1 Hi! " << i << std::endl;

		m2.unlock();
		m1.unlock();
	}
}

void worker2(std::mutex& m1, std::mutex& m2)
{
	for (int i = 0; i < 10; ++i)
	{
		while (true)
		{
			m1.lock();

			// 락이 실패하면
			if (!m2.try_lock()) {
				m1.unlock();
				continue;
			}

			std::cout << "Worker2 Hi! " << i << std::endl;

			m1.unlock();
			m2.unlock();
			break;
		}
	}
}

int main(int argc, int* argv[])
{
	std::mutex m1, m2;

	std::thread t1(worker1, std::ref(m1), std::ref(m2));
	std::thread t2(worker2, std::ref(m1), std::ref(m2));

	t1.join();
	t2.join();

	std::cout << "끝!" << std::endl;
};


