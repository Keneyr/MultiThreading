#pragma once
#include <mutex>
#include <thread>

template <typename T>
class Consumer
{
public:
	T thread_consume_item(const int thread_index, repo<T>& param_repo);
	void thread_con(const int thread_index, repo<T>* param_repo);
private:

};

//implementation
template <typename T>
T Consumer<T>::thread_consume_item(const int thread_index, repo<T>& param_repo)
{
	std::unique_lock<std::mutex> lock(param_repo._mtx_queue);

	// 1. ��������Ҫ�ȴ�����������Ϊ�ա����ź�
	param_repo._cv_queue_not_empty.wait(lock, [&] {return !param_repo._queue.empty(); });

	// 2. �ó�����
	T item;
	item = param_repo._queue.front();
	param_repo._queue.pop();

	std::cout << "������" << thread_index << "�ӻ��������ó�һ������:" << item << std::endl;

	// 3. ֪ͨ�����ߣ���������
	param_repo._cv_queue_not_full.notify_one();

	return item;
}

/**
*  @ brief: �������߳�
*  @ thread_index - �̱߳�ʶ�������߳�
*  @ param_repo - ������
*  @ return - void
*/
template< typename T >
void Consumer<T>::thread_con(const int thread_index, repo<T>* param_repo)
{
	if (nullptr == param_repo || NULL == param_repo)
		return;

	while (true)
	{
		bool is_running = true;
		{
			std::unique_lock<std::mutex> lock(param_repo->_mtx_con);
			// ��û���ѵ�ָ������Ŀ����������
			if (param_repo->_cnt_cur_con < cnt_total_10)
			{
				thread_consume_item<T>(thread_index, *param_repo);
				++param_repo->_cnt_cur_con;
			}
			else
				is_running = false;

		}

		std::this_thread::sleep_for(std::chrono::microseconds(16));

		// �����߳�
		if ((!is_running))
			break;
	}
}