#include <list>
#include <mutex>
#include <algorithm>
#include <thread>

std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int new_value)
{
	std::lock_guard<std::mutex> guard(some_mutex);
	some_list.push_back(new_value);
}
bool list_contains(int value_to_find)
{
	std::lock_guard<std::mutex> guard(some_mutex);
	return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}


int main()
{
	std::thread t1(add_to_list, 10);
	std::thread t2(list_contains, 10);
	t1.join();
	t2.join();

	system("pause");
}