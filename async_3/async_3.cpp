
#include<future>
#include <iostream>
#include <thread>
#include <sstream>
volatile int value = 0;


void foo()
{
	for (int i = 0; i < 100; ++i)
	{
		std::cout << std::this_thread::get_id() << std::endl;
	}
}
int main()
{
	std::cout << std::this_thread::get_id() << std::endl;

	auto s = std::async(std::launch::deferred,foo);
	s.wait();
	auto d = std::async(std::launch::deferred, foo);
	d.wait();
	auto c= std::async(std::launch::deferred,foo);
	c.wait();
}