
#include <future>
#include <iostream>
#include <thread>
#include <vector>

int sum(const std::vector<int>& v, int start, int end)
{
	int total = 0;
	for (int i = start; i < end; ++i)
	{
		total += v[i];
	}
	return total;
}

int parallel_sum(const std::vector<int>& v)
{
	std::future<int> lower_half_future =
		std::async(std::launch::async,sum, cref(v), 0, v.size() / 2);

	int upper_half = sum(v, v.size() / 2, v.size());

	return lower_half_future.get() + upper_half;
}

void start()
{
	std::vector<int> v;
	v.reserve(1000);

	for (int i = 0; i < 1000; ++i)
	{
		v.push_back(i + 1);
	}

	std::cout << "1 부터 1000 까지의 합 : " << parallel_sum(v) << std::endl;

}

volatile int temp{ 0 };
void foo()
{
	std::cout << "!";

	for (int i = 0; i < 10; ++i)
	{
		std::cout << std::this_thread::get_id() <<"실행!!" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
void foo2()
{
	std::cout << "!";

	for (int i = 0; i < 10; ++i)
	{
		std::cout << std::this_thread::get_id() << "실행2222!!" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
void foo3()
{
	std::cout << "!";

	for (int i = 0; i < 10; ++i)
	{
		std::cout << std::this_thread::get_id() << "실행3333!!" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
};

int main()
{
	std::async(foo);
	std::async(foo2);
	std::async(foo3);

	std::cout << temp;
}