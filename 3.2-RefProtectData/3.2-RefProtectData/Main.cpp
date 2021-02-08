//无意中传递了保护数据的引用

#include <iostream>
#include <thread>
#include <string>
#include <mutex>

class some_data
{
	int a;
	std::string b;
public:
	void do_something();
};

class data_wrapper
{
private:
	some_data data;
	std::mutex m;
public:
	template<typename Function>
	void process_data(Function func)
	{
		std::lock_guard<std::mutex> l(m);
		func(data); //传递"保护"数据给用户函数
	}
};

//这个共享数据没被保护，所以整体的这个功能，加锁白瞎
some_data* unprotected;
void malicious_function(some_data& protected_data)
{
	unprotected = &protected_data;
}

data_wrapper x;
void foo()
{
	x.process_data(malicious_function); //传递一个恶意函数
	unprotected->do_something(); //在无保护的情况下访问保护数据
}
int main()
{
	std::thread t1(foo);
	std::thread t2(foo);
	t1.join();
	t2.join();

	system("pause");
	return 0;
}