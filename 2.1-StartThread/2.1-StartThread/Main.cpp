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
			do_something(i); //Ǳ�ڷ���������������
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
	//�߳��������ڵĹ���
	try
	{

	}
	catch (...)
	{
		my_thread.join();
		throw;
	}
	my_thread.join(); //һ��Ҫ��join������detach
}
int main()
{
	oops();
	system("pause");
	return 0;
}