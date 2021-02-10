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
*  @ brief: 生产者线程
*  @ thread_index - 线程标识，区分是哪一个线程
*  @ count_max_produce - 最大生产次数
*  @ param_repo - 缓冲区
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
			// 用于生产者之间竞争
			std::unique_lock<std::mutex> lock(param_repo->_mtx_pro);

			// 缓冲区没有满，继续生产
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

	// 1. 生产者只要发现缓冲区没有满， 就继续生产
	param_repo._cv_queue_not_full.wait(lock, [&] { return param_repo._queue.size() < param_repo._count_max_queue_10; });

	// 2. 将生产好的商品放入缓冲区
	param_repo._queue.push(repo_item);

	// log to console
	std::cout << "生产者" << thread_index << "生产数据：" << repo_item << "\n";

	// 3. 通知消费者可以消费了
	//param_repo._cv_queue_not_empty.notify_one();
	param_repo._cv_queue_not_empty.notify_one();
}