#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <thread>
#include <string>
using namespace std;

void f(int i, string const& s)
{
	cout << i << s << endl;
}

void not_oops(int some_param)
{
	char buffer[1024];
	sprintf(buffer, "%i", some_param);
	//使用string，避免悬空指针
	thread t(f, 3, string(buffer));
	t.detach();
}
struct widget_id
{

};
struct widget_data
{

};
void update_data_for_widget(widget_id w, widget_data& data)
{

}
void process_widget_data(const widget_data& data)
{

}
void oops_again(widget_id w)
{
	widget_data data;
	//不加ref的话，thread构造函数不知道应该传递引用，会调用拷贝构造，增加开销
	thread t(update_data_for_widget, w, ref(data));
	t.join();
	process_widget_data(data);
}

class X
{
public:
	void do_lengthy_work(int& num)
	{

	}
	~X() {};
};

void oops_class()
{
	X my_x;
	int num(0);
	thread t(&X::do_lengthy_work, &my_x, ref(num));
	t.join();
}

class big_object
{
public:
	int t;
	void prepare_data(int num) { t = num; }
	big_object():t(1){}
};

void process_big_object(unique_ptr<big_object> pt)
{
	cout << pt->t << endl;
}
void oops_unique_ptr()
{
	unique_ptr<big_object> p(new big_object);
	p->prepare_data(42);
	thread t(process_big_object, move(p));
	t.join();
}

int main()
{
	int main_param = 20000;
	not_oops(main_param);
	
	widget_id w;
	oops_again(w);

	oops_class();
	
	oops_unique_ptr();

	system("pause");
}