#pragma once
//////////////////////////////////////////////////////////////////////////
//Master:�������񣬲����������Worker
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

	//���췽��,��ʼ������worker
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
	//������ύ
	void submit(Task task) {
		this->workerQueue.push(task);
	}
	void func()
	{
		worker->run();
	}
	//ִ������
	int execute() {
		for (int i = 0; i < workerCount; i++)
		{
			std::thread t(this->func);
			t.join();
		}
		return getResult();
	}
	//���������������ս��
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

	//��������Ķ���
	std::queue<Task> workerQueue;

	//����worker
	//std::unordered_map<std::string, std::thread::id> workers;

	//���������worker���صĽ��
	std::unordered_map<std::string, int> resultMap;

};