#include <future>
#include <iostream>
#include <thread>

int some_task(int x) { return 10 + x;  };

int main()
{
	std::packaged_task<int(int)> task(some_task);
	std::future<int> start = task.get_future();

	std::thread t(std::move(task),5);

	std::cout << "결과값 : " << start.get() << std::endl;

	t.join();
}