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

	// 1. 消费者需要等待【缓冲区不为空】的信号
	param_repo._cv_queue_not_empty.wait(lock, [&] {return !param_repo._queue.empty(); });

	// 2. 拿出数据
	T item;
	item = param_repo._queue.front();
	param_repo._queue.pop();

	std::cout << "消费者" << thread_index << "从缓冲区中拿出一组数据:" << item << std::endl;

	// 3. 通知生产者，继续生产
	param_repo._cv_queue_not_full.notify_one();

	return item;
}

/**
*  @ brief: 消费者线程
*  @ thread_index - 线程标识，区分线程
*  @ param_repo - 缓冲区
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
			// 还没消费到指定的数目，继续消费
			if (param_repo->_cnt_cur_con < cnt_total_10)
			{
				thread_consume_item<T>(thread_index, *param_repo);
				++param_repo->_cnt_cur_con;
			}
			else
				is_running = false;

		}

		std::this_thread::sleep_for(std::chrono::microseconds(16));

		// 结束线程
		if ((!is_running))
			break;
	}
}