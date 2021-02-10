#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include "Consumer.h"
#include "Producer.h"

std::mutex _mtx;
std::condition_variable _cv_not_full;
std::condition_variable _cv_not_empty;

const int max_queue_size_10 = 10;

enum
{
	// 总生产数目
	cnt_total_10 = 10,
};


template<typename T>
struct repo_
{
	// 用作互斥访问缓冲区
	std::mutex				_mtx_queue;

	// 缓冲区最大size
	unsigned int			_count_max_queue_10 = 10;

	// 缓冲区
	std::queue<T>			_queue;

	// 缓冲区没有满，通知生产者继续生产
	std::condition_variable _cv_queue_not_full;

	// 缓冲区不为空，通知消费者继续消费
	std::condition_variable _cv_queue_not_empty;



	// 用于生产者之间的竞争
	std::mutex				_mtx_pro;
	// 计算当前已经生产了多少数据了
	unsigned int			_cnt_cur_pro = 0;


	// 用于消费者之间的竞争
	std::mutex				_mtx_con;
	// 计算当前已经消费多少数据了
	unsigned int			_cnt_cur_con = 0;


	repo_(const unsigned int count_max_queue = 10) :_count_max_queue_10(count_max_queue)
		, _cnt_cur_con(0)

	{
		;
	}

	repo_(const repo_&instance) = delete;
	repo_& operator = (const repo_& instance) = delete;
	repo_(const repo_&&instance) = delete;
	repo_& operator = (const repo_&& instance) = delete;

};

template <typename T>
using repo = repo_<T>;

// 入口函数
//----------------------------------------------------------------------------------------

int main(int argc, char *argv[], char *env[])
{
	// 缓冲区
	repo<int> repository;
	// 线程池
	std::vector<std::thread> vec_thread;

	// 生产者
	vec_thread.push_back(std::thread(Producer<int>::thread_pro, 1, cnt_total_10, &repository));
	vec_thread.push_back(std::thread(Producer<int>::thread_pro, 2, cnt_total_10, &repository));

	// 消费者
	vec_thread.push_back(std::thread(Consumer<int>::thread_con, 1, &repository));
	vec_thread.push_back(std::thread(Consumer<int>::thread_con, 2, &repository));



	for (auto &item : vec_thread)
	{
		item.join();
	}

	return 0;
}
