#pragma once

#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>

#include <iostream>

#include "ThreadPool.h"
#include "LRUCache.h"

// �������
// Ŀǰ8����⣬2������
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

// �̳߳ش�������
static ThreadPool<std::string> threadPool(8, 12);