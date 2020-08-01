#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <future>


std::vector<int> vec{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 };


void work()
{

}
			// [start,end)

bool find(int start, int end, int target)
{
	for (int i = start; i < end; ++i)
	{
		std::cout << std::this_thread::get_id()<< " 가 찾는중\n";

		if ( vec[i] == target)
		{
			std::cout << "Search success !\n";
			return true;
		}
	}
	return false; 
}

bool async_find(int start, int end, int target, int asyncrous_num=1)
{

	int range = (end - start)/asyncrous_num;
	// 0~100
	std::vector<std::future<bool>> futures;
	futures.reserve(asyncrous_num);

	for (int i = 0; i < asyncrous_num; ++i)
	{
		int left = i * range;

		futures.push_back(std::async(std::launch::async,find,left,left+range,target));
	}

	for (auto& element : futures)
	{
		if (element.get()) {
			return true; 
		}
	}

	return false; 
}
int main()
{
	std::cout << std::boolalpha << async_find(0, 20, 19, 5);
}