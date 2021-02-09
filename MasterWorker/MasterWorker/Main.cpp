//////////////////////////////////////////////////////////////////////////
//多线程编程 设计模式中的 Master-Worker模式，一个小小的Demo
//参考博客：https://zhuanlan.zhihu.com/p/108215525?from_voters_page=true
//计算1 - 100的平方和
//目前还有错，我还不知道为啥，但是已经理解了master-worker设计模式
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <ctime>

#include "Master.h"
#include "MyWorker.h"
#include "Task.h"

//客户端Client
int main()
{
	Master master(10);

	//提交n个任务到任务队列里
	for (int i = 0; i < 100; i++) {
		Task task;
		task.setId(i);
		task.setName("任务" + i);
		task.setNum(i + 1);
		master.submit(task);
	}

	//执行任务
	long start = clock();
	int res = master.execute();
	long time = clock() - start;
	std::cout << "结果:" << res << ",耗时:" << time << std::endl;

	system("pause");
	return 0;
}

