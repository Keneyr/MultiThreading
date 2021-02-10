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
	// ��������Ŀ
	cnt_total_10 = 10,
};


template<typename T>
struct repo_
{
	// ����������ʻ�����
	std::mutex				_mtx_queue;

	// ���������size
	unsigned int			_count_max_queue_10 = 10;

	// ������
	std::queue<T>			_queue;

	// ������û������֪ͨ�����߼�������
	std::condition_variable _cv_queue_not_full;

	// ��������Ϊ�գ�֪ͨ�����߼�������
	std::condition_variable _cv_queue_not_empty;



	// ����������֮��ľ���
	std::mutex				_mtx_pro;
	// ���㵱ǰ�Ѿ������˶���������
	unsigned int			_cnt_cur_pro = 0;


	// ����������֮��ľ���
	std::mutex				_mtx_con;
	// ���㵱ǰ�Ѿ����Ѷ���������
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

// ��ں���
//----------------------------------------------------------------------------------------

int main(int argc, char *argv[], char *env[])
{
	// ������
	repo<int> repository;
	// �̳߳�
	std::vector<std::thread> vec_thread;

	// ������
	vec_thread.push_back(std::thread(Producer<int>::thread_pro, 1, cnt_total_10, &repository));
	vec_thread.push_back(std::thread(Producer<int>::thread_pro, 2, cnt_total_10, &repository));

	// ������
	vec_thread.push_back(std::thread(Consumer<int>::thread_con, 1, &repository));
	vec_thread.push_back(std::thread(Consumer<int>::thread_con, 2, &repository));



	for (auto &item : vec_thread)
	{
		item.join();
	}

	return 0;
}
