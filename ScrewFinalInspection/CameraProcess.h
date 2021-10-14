#pragma once
#ifndef CAMERAPROCESS_H
#define CAMERAPROCESS_H

#include "common.h"

// 获取螺丝分割图像
std::vector<cv::Mat> getThrdMat(cv::Mat &src) {
	cv::Mat gray;

	if (src.channels() != 1)
		cv::cvtColor(src, gray, CV_BGR2GRAY);
	else
		gray = src.clone();

	cv::Mat imageOtsuUp, imageOtsuDown;
	int upGrayImageUp = 0, upGrayImageDown = gray.rows / 2, upGrayImageLeft = gray.cols / 4, upGrayImageRight = gray.cols / 4 * 3;
	cv::Mat upGrayImage = gray(cv::Range(upGrayImageUp, upGrayImageDown),
		cv::Range(upGrayImageLeft, upGrayImageRight));
	cv::Mat downGrayImage = gray(cv::Range(upGrayImageDown, gray.rows),
		cv::Range(upGrayImageLeft, upGrayImageRight));

	cv::threshold(upGrayImage, imageOtsuUp, 0, 255, CV_THRESH_OTSU);
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11), cv::Point(-1, -1));
	cv::erode(imageOtsuUp, imageOtsuUp, kernel);

	// 投影到下边统计
	std::vector<int> upColsCount(imageOtsuUp.cols, 0);
	for (int i = 0; i < imageOtsuUp.rows; ++i) {
		for (int j = 0; j < imageOtsuUp.cols; ++j) {
			if (imageOtsuUp.at<uchar>(i, j) == 0) {
				upColsCount[j] += 1;
			}
		}
	}

	int maxColCount = *std::max_element(upColsCount.begin(), upColsCount.end());
	int minColCount = *std::min_element(upColsCount.begin(), upColsCount.end());
	int upColThrd = (maxColCount + minColCount) / 2;
	int upLeft = 0, upRight = 0, upUp = 0;	// 后续需要的三个参数
	for (int i = 0; i < upColsCount.size(); ++i) {	// 从左向右找到左边界
		if (upColsCount[i] > upColThrd) {
			upLeft = i;
			break;
		}
	}
	for (int i = upColsCount.size() - 1; i >= 0; --i) {	// 从右向左找到右边界
		if (upColsCount[i] > upColThrd) {
			upRight = i;
			break;
		}
	}
	// 从两个边界的中间从下往上找，找到不是黑的作为第三个参数
	int tmpMid = (upLeft + upRight) / 2;
	for (int i = imageOtsuUp.rows - 1; i >= 0; --i) {
		if (imageOtsuUp.at<uchar>(i, tmpMid) != 0) {
			upUp = i;
			break;
		}
	}
	std::cout << "upLeft:" << upLeft << " upRight:" << upRight << " upUp:" << upUp << std::endl;

	cv::threshold(downGrayImage, imageOtsuDown, 0, 255, CV_THRESH_OTSU);
	kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(19, 19), cv::Point(-1, -1));
	cv::erode(imageOtsuDown, imageOtsuDown, kernel);

	// 投影到左边进行统计
	std::vector<int> downRowsCount(imageOtsuDown.rows, 0);
	std::vector<int> downColsCount(imageOtsuDown.cols, 0);
	for (int i = 0; i < imageOtsuDown.rows; ++i) {
		for (int j = 0; j < imageOtsuDown.cols; ++j) {
			if (imageOtsuDown.at<uchar>(i, j) == 0) {
				downRowsCount[i] += 1;
				downColsCount[j] += 1;
			}
		}
	}

	int upDown, midLeft, midRight, midDown;

	bool hasFindUp = false;
	for (int i = 0; i < downRowsCount.size(); ++i) {
		if (!hasFindUp) {
			if (downRowsCount[i] == 0) {	// 找到中间的上边界
				upDown = i;
				hasFindUp = true;
			}
		}
		else {
			if (downRowsCount[i] >(upRight - upLeft)) {	// 找到中间的下边界
				midDown = i;
				break;
			}
		}
	}

	// 找到中间的左右边界

	for (int j = 0; j < imageOtsuDown.cols; ++j) {
		if (imageOtsuDown.at<uchar>(midDown, j) == 0) {
			midLeft = j;
			break;
		}
	}
	for (int j = imageOtsuDown.cols - 1; j >= 0; --j) {
		if (imageOtsuDown.at<uchar>(midDown, j) == 0) {
			midRight = j;
			break;
		}
	}

	std::cout << "upDown:" << upDown << " midLeft:" << midLeft << " midRight:" << midRight << " midDown:" << midDown << std::endl;

	cv::Mat upMat = gray(cv::Range(upGrayImageUp + upUp, upGrayImageDown + upDown),
		cv::Range(upGrayImageLeft + upLeft, upGrayImageLeft + upRight));

	cv::Mat midMat = gray(cv::Range(upGrayImageDown + upDown, upGrayImageDown + midDown),
		cv::Range(upGrayImageLeft + midLeft, upGrayImageLeft + midRight));

	int downLeft, downRight, downUp, downDown;

	downUp = midDown + (midDown - upDown) / 2;
	for (int i = downUp; i < downRowsCount.size(); ++i) {
		if (downRowsCount[i] >(midRight - midLeft)) {	// 找到下边的下边界
			downDown = i;
			break;
		}
	}

	int downMid = (downUp + downDown) / 2;

	for (int j = 0; j < imageOtsuDown.cols; ++j) {
		if (imageOtsuDown.at<uchar>(downMid, j) == 0) {
			downLeft = j;
			break;
		}
	}
	for (int j = imageOtsuDown.cols - 1; j >= 0; --j) {
		if (imageOtsuDown.at<uchar>(downMid, j) == 0) {
			downRight = j;
			break;
		}
	}

	std::cout << "downUp:" << downUp << " downLeft:" << downLeft << " downRight:" << downRight << " downDown:" << downDown << std::endl;

	cv::Mat downMat = gray(cv::Range(upGrayImageDown + downUp, upGrayImageDown + downDown),
		cv::Range(upGrayImageLeft + downLeft, upGrayImageLeft + downRight));

	std::vector<cv::Mat> ans;
	ans.push_back(upMat);
	ans.push_back(midMat);
	ans.push_back(downMat);

	return ans;
}

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