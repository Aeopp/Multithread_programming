#include <future>
#include <thread>
#include <iostream>

void foo()
{
	for (int i = 0; i < 100000000; ++i)
		std::cout << std::this_thread::get_id() << std::endl;
};

struct z  : std::future<void> 
{

};

void bar()
{
	std::packaged_task<void()> p(foo);
	std::packaged_task<void()> p1(foo);
	std::packaged_task<void()> p2(foo);

	auto a = std::thread(std::move(p));
	auto a2 = std::thread(std::move(p1));
	auto a3 = std::thread(std::move(p2));

	a.detach();
	a2.detach();
	a3.detach();
}

int main()
{
	bar();

	std::this_thread::sleep_for(std::chrono::seconds(1));

}