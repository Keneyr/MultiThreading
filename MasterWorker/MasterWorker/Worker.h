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
		
		//从任务队列中取出一个任务
		Task task = workerQueue.front();
		//if (task == nullptr) break;
			
		//处理具体的任务,注意这里多态的话，调用的是哪个类下的doTask
		int res = doTask(task);
			
		//把每次处理的结果放到结果集里面，此处直接把num值作为结果
		resultMap[std::to_string(task.getId())] = res;
		
	}
	virtual int doTask(Task task) {
		return 1;
	}
};