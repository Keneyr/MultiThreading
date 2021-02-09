#pragma once
//////////////////////////////////////////////////////////////////////////
//Master:接受任务，并分配任务给Worker
//////////////////////////////////////////////////////////////////////////
#include <string>
#include <map>
#include <queue>
#include <unordered_map>
#include <thread>
#include "Task.h"
#include "Worker.h"
#include "MyWorker.h"
class Master {

public:

	//构造方法,初始化所有worker
	Master(int workerCount){
		this->workerCount = workerCount;
		this->worker = new MyWorker();
		worker->setWorkerQueue(this->workerQueue);
		worker->setResultMap(this->resultMap);
	}
	~Master()
	{
		delete worker;
	}
	//任务的提交
	void submit(Task task) {
		this->workerQueue.push(task);
	}
	void func()
	{
		worker->run();
	}
	//执行任务
	int execute() {
		for (int i = 0; i < workerCount; i++)
		{
			std::thread t(this->func);
			t.join();
		}
		return getResult();
	}
	//处理结果集返回最终结果
	int getResult() {
		int res = 0;
		for (auto& entry : resultMap)
		{
			res += entry.second;
		}
		return res;
	}
private:
	Worker* worker = nullptr;

	int workerCount;

	//所有任务的队列
	std::queue<Task> workerQueue;

	//所有worker
	//std::unordered_map<std::string, std::thread::id> workers;

	//共享变量，worker返回的结果
	std::unordered_map<std::string, int> resultMap;

};