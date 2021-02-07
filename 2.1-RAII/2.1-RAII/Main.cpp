//���Զϵ�����
#include <iostream>
#include <thread>
using namespace std;

class thread_guard
{
	thread& t;
public:
	explicit thread_guard(thread& t_) :t(t_) {}
	~thread_guard()
	{
		if (t.joinable())
		{
			t.join();
		}
	}
	//Ϊ�˲��ñ������Զ����ɡ�ֱ�ӶԶ�����п�����ֵ�Ǻ�Σ�յģ���Ϊ����ܻ�Ū���ѻ�����߳�
	thread_guard(thread_guard const&) = delete;
	thread_guard& operator=(thread_guard const&) = delete;
};

struct func
{
	int& i;
	func(int& i_):i(i_){}
	void operator()()
	{
		for (unsigned j = 0; j < 10000; ++j)
		{
			do_something(i);
		}
	}
	void do_something(int i)
	{
		cout << i << endl;
	}
};

void do_something_in_current_thread()
{
	cout << "hhh" << endl;
}
void f()
{
	int some_local_state = 0;
	func my_func(some_local_state);
	thread t(my_func);
	thread_guard g(t);
	do_something_in_current_thread();
}

int main()
{
	f();
}