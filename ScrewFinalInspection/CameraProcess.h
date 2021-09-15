#pragma once
#ifndef CAMERAPROCESS_H
#define CAMERAPROCESS_H

#include "common.h"

/*
下面为8个缺陷 + 2个测量的处理函数
*/

static int processFlaw1(cv::Mat &src, int mCamera) {
	std::cout << " [in] Flaw 1";
	std::chrono::milliseconds dura(600);
	std::this_thread::sleep_for(dura);

	cv::imwrite(".\\Result\\" + std::to_string(mCamera) + "_Flaw_1.jpg", src);
	return 1;
}

static int processFlaw2(cv::Mat &src, int mCamera) {
	std::cout << " [in] Flaw 2";
	std::chrono::milliseconds dura(600);
	std::this_thread::sleep_for(dura);

	cv::imwrite(".\\Result\\" + std::to_string(mCamera) + "_Flaw_2.jpg", src);
	return 1;
}

static int processFlaw3(cv::Mat &src, int mCamera) {
	std::cout << " [in] Flaw 3";
	std::chrono::milliseconds dura(600);
	std::this_thread::sleep_for(dura);

	cv::imwrite(".\\Result\\" + std::to_string(mCamera) + "_Flaw_3.jpg", src);
	return 1;
}

static int processFlaw4(cv::Mat &src, int mCamera) {
	std::cout << " [in] Flaw 4";
	std::chrono::milliseconds dura(600);
	std::this_thread::sleep_for(dura);

	cv::imwrite(".\\Result\\" + std::to_string(mCamera) + "_Flaw_4.jpg", src);
	return 1;
}

static int processFlaw5(cv::Mat &src, int mCamera) {
	std::cout << " [in] Flaw 5";
	std::chrono::milliseconds dura(600);
	std::this_thread::sleep_for(dura);

	cv::imwrite(".\\Result\\" + std::to_string(mCamera) + "_Flaw_5.jpg", src);
	return 1;
}

static int processFlaw6(cv::Mat &src, int mCamera) {
	std::cout << " [in] Flaw 6";
	std::chrono::milliseconds dura(600);
	std::this_thread::sleep_for(dura);

	cv::imwrite(".\\Result\\" + std::to_string(mCamera) + "_Flaw_6.jpg", src);
	return 1;
}

static int processFlaw7(cv::Mat &src, int mCamera) {
	std::cout << " [in] Flaw 7";
	std::chrono::milliseconds dura(600);
	std::this_thread::sleep_for(dura);

	cv::imwrite(".\\Result\\" + std::to_string(mCamera) + "_Flaw_7.jpg", src);
	return 1;
}

static int processFlaw8(cv::Mat &src, int mCamera) {
	std::cout << " [in] Flaw 8";
	std::chrono::milliseconds dura(600);
	std::this_thread::sleep_for(dura);

	cv::imwrite(".\\Result\\" + std::to_string(mCamera) + "_Flaw_8.jpg", src);
	return 1;
}

static int processMeasure1(cv::Mat &src, int mCamera) {
	std::cout << " [in] Measure 1";
	std::chrono::milliseconds dura(600);
	std::this_thread::sleep_for(dura);

	cv::imwrite(".\\Result\\" + std::to_string(mCamera) + "_Measure_1.jpg", src);
	return 99 + rand() % 100;
}

static int processMeasure2(cv::Mat &src, int mCamera) {
	std::cout << " [in] Measure 2";
	std::chrono::milliseconds dura(600);
	std::this_thread::sleep_for(dura);

	cv::imwrite(".\\Result\\" + std::to_string(mCamera) + "_Measure_2.jpg", src);
	return 98 + rand() % 100;
}

#endif