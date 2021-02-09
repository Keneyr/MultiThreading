//////////////////////////////////////////////////////////////////////////
//���̱߳�� ���ģʽ�е� Master-Workerģʽ��һ��СС��Demo
//�ο����ͣ�https://zhuanlan.zhihu.com/p/108215525?from_voters_page=true
//����1 - 100��ƽ����
//Ŀǰ���д��һ���֪��Ϊɶ�������Ѿ������master-worker���ģʽ
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <ctime>

#include "Master.h"
#include "MyWorker.h"
#include "Task.h"

//�ͻ���Client
int main()
{
	Master master(10);

	//�ύn���������������
	for (int i = 0; i < 100; i++) {
		Task task;
		task.setId(i);
		task.setName("����" + i);
		task.setNum(i + 1);
		master.submit(task);
	}

	//ִ������
	long start = clock();
	int res = master.execute();
	long time = clock() - start;
	std::cout << "���:" << res << ",��ʱ:" << time << std::endl;

	system("pause");
	return 0;
}

