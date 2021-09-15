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
内存共享区

相机编号：idx%cameraNum 相等的实际都是同一个相机拍的
*/
class SingleMemory {
private:
	// 私有化构造函数
	SingleMemory() {};   
	SingleMemory(const SingleMemory &) {};  
	SingleMemory& operator=(const SingleMemory &) {};  

	static SingleMemory* uniqueInstance;
	static std::mutex mMutex;

	// <相机编号, 抓拍图像>缓存区域
	LRUCache<int, cv::Mat> *mCache;
	// <相机编号, 图像已经检测次数>缓存区域
	std::unordered_map<int, int> *mDetectCounts;
	// <相机编号, 图像各类检测情况>缓存区域
	std::unordered_map<int, std::vector<int>> *mDetectStatus;

	int detectTypeNums = 0;

	//
	std::queue<int> *mReadCameraBuffer;

public:
	// 单例接口
	static SingleMemory *&getInstance() {
		if (uniqueInstance == nullptr) {
			std::unique_lock<std::mutex> lock(mMutex);
			if (uniqueInstance == nullptr) {   // 当为空的时候，创建一次
				uniqueInstance = new SingleMemory();
			}
		}
		return uniqueInstance; // 返回创建的类对象
	}

	// 初始化mCache缓存大小，大小为相机数量*2
	void initSingleMemorySize(int mCameraSize, int detectTypeNums) {
		mCache = new LRUCache<int, cv::Mat>(mCameraSize * 2);
		mDetectCounts = new std::unordered_map<int, int>();
		mDetectStatus = new std::unordered_map<int, std::vector<int>>();
		mReadCameraBuffer = new std::queue<int>();

		this->detectTypeNums = detectTypeNums;
	}

	// 根据相机编号，获取对应的图像
	cv::Mat getLRUCacheMatByCameraIndex(int mCamera) {
		return mCache->get(mCamera);
	}

	// 添加<相机编号, 抓拍图像>进入缓存
	void putLRUCacheCameraIndexAndMat(int mCamera, cv::Mat mat) {
		mCache->put(mCamera, mat);
	}

	// 判断缓存中是否有某个相机编号的图像
	bool LRUCacheCameraAndMatContains(int mCamera) {
		return mCache->contains(mCamera);
	}

	// 初始化统计检测结果区
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

	// 更新检测结果
	void setDetectResult(int mCamera, int detectCate, int detectResult) {
		mDetectCounts->at(mCamera)--;
		mDetectStatus->at(mCamera)[detectCate] = detectResult;

		if (mDetectCounts->at(mCamera) == 0) {
			mReadCameraBuffer->push(mCamera);
		}
	}

	// 获取当前计算结果进度
	int getCurCameraDetectProcess(int mCamera) {
		return mDetectCounts->at(mCamera);
	}

	// 获取当前检测结果
	std::vector<int> getCurCameraDetectResult(int mCamera) {
		return mDetectStatus->at(mCamera);
	}

	// 获取已经处理好的相机编号
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