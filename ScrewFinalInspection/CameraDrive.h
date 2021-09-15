#pragma once
#ifndef CAMERADRIVE_H
#define CAMERADRIVE_H

#include "common.h"
#include "MvCameraControl.h"

#include <sys/timeb.h>

class CameraDrive {
public:
	CameraDrive();
	~CameraDrive();

	// ��ȡ�ⲿ������������
	const int getExternalCameraNum() const;

	// ��ʼ���������ΪӲ������
	bool camerasInitInHardTriggerMode() const;

	// ��ʼ����ץͼ�ص�����
	bool startGrabForExternalTrigger() const;

private:

	bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo) const;

	// convert data stream in Mat format
	bool Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char * pData, cv::Mat & srcImg) const;

	int RGB2BGR(unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight) const;
};

#endif