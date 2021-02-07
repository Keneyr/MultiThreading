#include <iostream>
#include <thread>
using namespace std;

void some_function() {}
void some_other_function() {}


int main()
{
	thread t1(some_function);
	thread t2 = move(t1);
	t1 = thread(some_other_function);

	thread t3;
	t3 = move(t2);
	//t1 = move(t3);

	system("pause");
}