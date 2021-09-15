#pragma once

#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>

#include <iostream>

#include "ThreadPool.h"
#include "LRUCache.h"

// 检测类型
// 目前8个检测，2个测量
const int detectTypeNum = 10;
enum DETECTTYPE
{
	FLAW1,
	FLAW2,
	FLAW3,
	FLAW4,
	FLAW5,
	FLAW6,
	FLAW7,
	FLAW8,
	MEASURE1,
	MEASURE2
};

// 线程池处理任务
static ThreadPool<std::string> threadPool(8, 12);