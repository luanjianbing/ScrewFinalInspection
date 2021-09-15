#pragma once
#ifndef THREADPOOL_H
#define THREADPOOL_H

//#include"common.h"
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
//#include <Windows.h>

#include "CameraProcess.h"
#include "SingleMemory.h"

class Semaphore {
public:
	Semaphore(int value = 1) : count{ value }, wakeups{ 0 } {}

	void wait() {
		std::unique_lock<std::mutex> lock{ mutex };
		if (--count<0) { // count is not enough ?
			condition.wait(lock, [&]()->bool { return wakeups>0; }); // suspend and wait ...
			--wakeups;  // ok, me wakeup !
		}
	}
	void signal() {
		std::lock_guard<std::mutex> lock{ mutex };
		if (++count <= 0) { // have some thread suspended ?
			++wakeups;
			condition.notify_one(); // notify one !
		}
	}
private:
	int count;
	int wakeups;
	std::mutex mutex;
	std::condition_variable condition;
};

template <typename T>
class ThreadPool {
private:
	//CameraProcess m_camProcess;
public:
	// threadNum是线程池中线程的数量
	// maxWaitImageNum是请求队列中最多允许等待处理的图像数目
	ThreadPool(int threadNum = 8, int maxWaitImageNum = 12) : m_threadNum(threadNum), m_maxWaitImageNum(maxWaitImageNum) {
		if (m_threadNum <= 0 || m_maxWaitImageNum <= 0)
			throw std::exception();
		//线程初始化
		m_threads = new std::thread[m_threadNum];
		if (!m_threads)
			throw std::exception();
		for (int i = 0; i < m_threadNum; ++i) {
			// 创建线程
			m_threads[i] = std::thread(worker, this);
			// detach线程
			m_threads[i].detach();
		}
	}
	~ThreadPool() {
		delete[] m_threads;
	}
	bool append_p(T request);
private:
	
	/*工作线程运行的函数，它不断从工作队列中取出任务并执行之*/
	static void *worker(void *arg);
	void run();
private:
	int m_threadNum = 0;	// 线程池中的线程数量
	int m_maxWaitImageNum = 0;	// 队列中的最大等待处理的图像数
	std::thread *m_threads;	// 描述线程池的数组
	std::queue<T> m_workQueue;	// 请求队列
	std::mutex m_queueLocker;	// 保护请求队列的互斥锁
	Semaphore m_queueState;	// 是否有任务需要处理
};

template <typename T>
bool ThreadPool<T>::append_p(T request) {
	m_queueLocker.lock();
	if (m_workQueue.size() >= m_maxWaitImageNum) {
		m_queueLocker.unlock();
		return false;
	}
	m_workQueue.push(request);
	m_queueLocker.unlock();
	m_queueState.signal();
	return true;
}

template <typename T>
void ThreadPool<T>::run() {
	// 线程抢占处理程序
	while (true) {
		m_queueState.wait();	// 等待信号量任务
		m_queueLocker.lock();
		if (m_workQueue.empty()) {
			m_queueLocker.unlock();
			continue;
		}
		T item = m_workQueue.front();	// 取出任务
		m_workQueue.pop();
		m_queueLocker.unlock();
		// 拿到了需要的信息
		//std::cout << "tid:[" << std::this_thread::get_id() << "] " << item << "[start]" << std::endl;

		if (typeid(item).name() == typeid(std::string).name()) {
			size_t pos = item.find(' ');
			int mCamera = atoi(item.substr(0, pos).c_str());
			std::cout << "mCamera:" << mCamera;

			DETECTTYPE mDetectType = DETECTTYPE(atoi(item.substr(pos + 1, item.size()).c_str()));
			// 获取图像副本
			cv::Mat thisMatClone = SingleMemory::getInstance()->getLRUCacheMatByCameraIndex(mCamera).clone();
			
			// 处理图像以及结果
			int mDetectAns = 0;
			// 处理状态
			switch (mDetectType)
			{
				case FLAW1: {
					mDetectAns = processFlaw1(thisMatClone, mCamera);
					break;
				}
				case FLAW2: {
					mDetectAns = processFlaw2(thisMatClone, mCamera);
					break;
				}
				case FLAW3: {
					mDetectAns = processFlaw3(thisMatClone, mCamera);
					break;
				}
				case FLAW4: {
					mDetectAns = processFlaw4(thisMatClone, mCamera);
					break;
				}
				case FLAW5: {
					mDetectAns = processFlaw5(thisMatClone, mCamera);
					break;
				}
				case FLAW6: {
					mDetectAns = processFlaw6(thisMatClone, mCamera);
					break;
				}
				case FLAW7: {
					mDetectAns = processFlaw7(thisMatClone, mCamera);
					break;
				}
				case FLAW8: {
					mDetectAns = processFlaw8(thisMatClone, mCamera);
					break;
				}
				case MEASURE1: {
					mDetectAns = processMeasure1(thisMatClone, mCamera);
					break;
				}
				case MEASURE2: {
					mDetectAns = processMeasure2(thisMatClone, mCamera);
					break;
				}
				default:
					break;
			}

			SingleMemory::getInstance()->setDetectResult(mCamera, mDetectType, mDetectAns);
			if (SingleMemory::getInstance()->getCurCameraDetectProcess(mCamera) == 0) {	// 这个相机已经处理完了
				std::cout << mCamera << " end" << mDetectAns << std::endl;
			}


			std::cout << " mDetectAns:" << mDetectAns << std::endl;
		}

		//std::cout << "tid:[" << std::this_thread::get_id() << "] " << item << "[end]" << std::endl;
	}
}

template <typename T>
void *ThreadPool<T>::worker(void *arg) {
	ThreadPool *pool = (ThreadPool *)arg;
	pool->run();
	return pool;
}
#endif