#pragma once
#include <mutex>
#include <thread>

template<typename T>
class Producer
{
public:
	void thread_pro(const int thread_index, const int count_max_produce, repo<T>* param_repo);
	void thread_produce_item(const int &thread_index, repo<T>& param_repo, const T& repo_item);

private:
	std::mutex mu;

};

//implementation

/**
*  @ brief: �������߳�
*  @ thread_index - �̱߳�ʶ����������һ���߳�
*  @ count_max_produce - �����������
*  @ param_repo - ������
*  @ return - void
*/
template<typename T>
void Producer<T>::thread_pro(const int thread_index, const int count_max_produce, repo<T>* param_repo)
{
	if (param_repo == nullptr || param_repo == NULL)
		return;
	while (true)
	{
		bool is_running = true;

		{
			// ����������֮�侺��
			std::unique_lock<std::mutex> lock(param_repo->_mtx_pro);

			// ������û��������������
			if (param_repo->_cnt_cur_pro < cnt_total_10)
			{
				thread_produce_item<T>(thread_index, *param_repo, param_repo->_cnt_cur_pro);
				++param_repo->_cnt_cur_pro;
			}
			else
				is_running = false;
		}

		std::this_thread::sleep_for(std::chrono::microseconds(16));
		if (!is_running)
			break;
	}
}

template<typename T>
void thread_produce_item(const int &thread_index, repo<T>& param_repo, const T& repo_item)
{
	std::unique_lock<std::mutex> lock(param_repo._mtx_queue);

	// 1. ������ֻҪ���ֻ�����û������ �ͼ�������
	param_repo._cv_queue_not_full.wait(lock, [&] { return param_repo._queue.size() < param_repo._count_max_queue_10; });

	// 2. �������õ���Ʒ���뻺����
	param_repo._queue.push(repo_item);

	// log to console
	std::cout << "������" << thread_index << "�������ݣ�" << repo_item << "\n";

	// 3. ֪ͨ�����߿���������
	//param_repo._cv_queue_not_empty.notify_one();
	param_repo._cv_queue_not_empty.notify_one();
}