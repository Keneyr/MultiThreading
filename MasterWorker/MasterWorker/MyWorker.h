#pragma once
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

#include "Worker.h"
#include <thread>
#include <memory>
#include <exception>

class MyWorker :public Worker {
public:
		int doTask(Task task) {
			
			//�������ֵ�ƽ��
			int num = task.getNum();
			return num * num;
	}
};