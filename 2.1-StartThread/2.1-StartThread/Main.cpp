#include <iostream>
#include <thread>
using namespace std;

struct func
{
	int& i;
	func(int& i_):i(i_){}
	void operator()()
	{
		for (unsigned j = 0; j < 1000; ++j)
		{
			do_something(i); //潜在访问隐患：空引用
		}
	}
	void do_something(int i)
	{
		cout << i << endl;
	}
};

void oops()
{
	int some_local_state = 0;
	func my_func(some_local_state);
	thread my_thread(my_func);
	//线程生命周期的管理
	try
	{

	}
	catch (...)
	{
		my_thread.join();
		throw;
	}
	my_thread.join(); //一定要用join而不是detach
}
int main()
{
	oops();
	system("pause");
	return 0;
}