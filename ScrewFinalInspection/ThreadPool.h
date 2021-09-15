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
	// threadNum���̳߳����̵߳�����
	// maxWaitImageNum������������������ȴ������ͼ����Ŀ
	ThreadPool(int threadNum = 8, int maxWaitImageNum = 12) : m_threadNum(threadNum), m_maxWaitImageNum(maxWaitImageNum) {
		if (m_threadNum <= 0 || m_maxWaitImageNum <= 0)
			throw std::exception();
		//�̳߳�ʼ��
		m_threads = new std::thread[m_threadNum];
		if (!m_threads)
			throw std::exception();
		for (int i = 0; i < m_threadNum; ++i) {
			// �����߳�
			m_threads[i] = std::thread(worker, this);
			// detach�߳�
			m_threads[i].detach();
		}
	}
	~ThreadPool() {
		delete[] m_threads;
	}
	bool append_p(T request);
private:
	
	/*�����߳����еĺ����������ϴӹ���������ȡ������ִ��֮*/
	static void *worker(void *arg);
	void run();
private:
	int m_threadNum = 0;	// �̳߳��е��߳�����
	int m_maxWaitImageNum = 0;	// �����е����ȴ������ͼ����
	std::thread *m_threads;	// �����̳߳ص�����
	std::queue<T> m_workQueue;	// �������
	std::mutex m_queueLocker;	// ����������еĻ�����
	Semaphore m_queueState;	// �Ƿ���������Ҫ����
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
	// �߳���ռ�������
	while (true) {
		m_queueState.wait();	// �ȴ��ź�������
		m_queueLocker.lock();
		if (m_workQueue.empty()) {
			m_queueLocker.unlock();
			continue;
		}
		T item = m_workQueue.front();	// ȡ������
		m_workQueue.pop();
		m_queueLocker.unlock();
		// �õ�����Ҫ����Ϣ
		//std::cout << "tid:[" << std::this_thread::get_id() << "] " << item << "[start]" << std::endl;

		if (typeid(item).name() == typeid(std::string).name()) {
			size_t pos = item.find(' ');
			int mCamera = atoi(item.substr(0, pos).c_str());
			std::cout << "mCamera:" << mCamera;

			DETECTTYPE mDetectType = DETECTTYPE(atoi(item.substr(pos + 1, item.size()).c_str()));
			// ��ȡͼ�񸱱�
			cv::Mat thisMatClone = SingleMemory::getInstance()->getLRUCacheMatByCameraIndex(mCamera).clone();
			
			// ����ͼ���Լ����
			int mDetectAns = 0;
			// ����״̬
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
			if (SingleMemory::getInstance()->getCurCameraDetectProcess(mCamera) == 0) {	// �������Ѿ���������
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