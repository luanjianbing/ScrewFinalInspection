#pragma once

#ifndef SINGLEMEMORY_H
#define SINGLEMEMORY_H

#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>

#include <iostream>

#include "LRUCache.h"

#include <unordered_map>
/*
�ڴ湲����

�����ţ�idx%cameraNum ��ȵ�ʵ�ʶ���ͬһ������ĵ�
*/
class SingleMemory {
private:
	// ˽�л����캯��
	SingleMemory() {};   
	SingleMemory(const SingleMemory &) {};  
	SingleMemory& operator=(const SingleMemory &) {};  

	static SingleMemory* uniqueInstance;
	static std::mutex mMutex;

	// <������, ץ��ͼ��>��������
	LRUCache<int, cv::Mat> *mCache;
	// <������, ͼ���Ѿ�������>��������
	std::unordered_map<int, int> *mDetectCounts;
	// <������, ͼ����������>��������
	std::unordered_map<int, std::vector<int>> *mDetectStatus;

	int detectTypeNums = 0;

	//
	std::queue<int> *mReadCameraBuffer;

public:
	// �����ӿ�
	static SingleMemory *&getInstance() {
		if (uniqueInstance == nullptr) {
			std::unique_lock<std::mutex> lock(mMutex);
			if (uniqueInstance == nullptr) {   // ��Ϊ�յ�ʱ�򣬴���һ��
				uniqueInstance = new SingleMemory();
			}
		}
		return uniqueInstance; // ���ش����������
	}

	// ��ʼ��mCache�����С����СΪ�������*2
	void initSingleMemorySize(int mCameraSize, int detectTypeNums) {
		mCache = new LRUCache<int, cv::Mat>(mCameraSize * 2);
		mDetectCounts = new std::unordered_map<int, int>();
		mDetectStatus = new std::unordered_map<int, std::vector<int>>();
		mReadCameraBuffer = new std::queue<int>();

		this->detectTypeNums = detectTypeNums;
	}

	// ���������ţ���ȡ��Ӧ��ͼ��
	cv::Mat getLRUCacheMatByCameraIndex(int mCamera) {
		return mCache->get(mCamera);
	}

	// ���<������, ץ��ͼ��>���뻺��
	void putLRUCacheCameraIndexAndMat(int mCamera, cv::Mat mat) {
		mCache->put(mCamera, mat);
	}

	// �жϻ������Ƿ���ĳ�������ŵ�ͼ��
	bool LRUCacheCameraAndMatContains(int mCamera) {
		return mCache->contains(mCamera);
	}

	// ��ʼ��ͳ�Ƽ������
	void reSetMapStatus(int mCamera, int taskNum) {
		if (mDetectCounts->find(mCamera) == mDetectCounts->end()) {
			mDetectCounts->insert({ mCamera, taskNum });
			std::vector<int> tmp(this->detectTypeNums, 0);
			mDetectStatus->insert({ mCamera, tmp });
		}
		else {
			mDetectCounts->at(mCamera) = 0;
			for (auto iter = mDetectStatus->at(mCamera).begin(); iter != mDetectStatus->at(mCamera).end(); ++iter)
				*iter = 0;
		}
	}

	// ���¼����
	void setDetectResult(int mCamera, int detectCate, int detectResult) {
		mDetectCounts->at(mCamera)--;
		mDetectStatus->at(mCamera)[detectCate] = detectResult;

		if (mDetectCounts->at(mCamera) == 0) {
			mReadCameraBuffer->push(mCamera);
		}
	}

	// ��ȡ��ǰ����������
	int getCurCameraDetectProcess(int mCamera) {
		return mDetectCounts->at(mCamera);
	}

	// ��ȡ��ǰ�����
	std::vector<int> getCurCameraDetectResult(int mCamera) {
		return mDetectStatus->at(mCamera);
	}

	// ��ȡ�Ѿ�����õ�������
	int getFinishedCamera() {
		if (!mReadCameraBuffer->empty()) {
			int ft = mReadCameraBuffer->front();
			mReadCameraBuffer->pop();
			return ft;
		}
		return -1;
	}
};

#endif