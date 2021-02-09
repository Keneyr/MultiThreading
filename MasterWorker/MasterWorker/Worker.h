#pragma once
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include <string>
#include <map>
#include <queue>
#include <unordered_map>
#include <thread>
#include "Task.h"
class Worker{
private:

	std::queue<Task> workerQueue;

	std::unordered_map<std::string, int> resultMap;
public:

	void setWorkerQueue(std::queue<Task> workerQueue) {
		this->workerQueue = workerQueue;
	}

	void setResultMap(std::unordered_map<std::string, int> resultMap) {
		this->resultMap = resultMap;
	}

	void run() {
		
		//�����������ȡ��һ������
		Task task = workerQueue.front();
		//if (task == nullptr) break;
			
		//������������,ע�������̬�Ļ������õ����ĸ����µ�doTask
		int res = doTask(task);
			
		//��ÿ�δ���Ľ���ŵ���������棬�˴�ֱ�Ӱ�numֵ��Ϊ���
		resultMap[std::to_string(task.getId())] = res;
		
	}
	virtual int doTask(Task task) {
		return 1;
	}
};