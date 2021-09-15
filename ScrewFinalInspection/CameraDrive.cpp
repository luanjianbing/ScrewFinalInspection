#include "CameraDrive.h"

static std::vector<void *> handle;	// ���������
static std::vector<unsigned int> g_nPayloadSize;	// ��������Ĵ�С

CameraDrive::CameraDrive() {

}

CameraDrive::~CameraDrive() {

}

// ���1Ӳ�������ص�����
void __stdcall ImageCallBackEx_0(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser) {
	cv::Mat tmpGrabImg_0 = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
	//std::string path = "./Image/ImageForGrab/";
	//// ��ȡʱ���
	//timeb t;
	//ftime(&t);
	//long long tms = std::move(t.time * 1000 + t.millitm);
	//path += (std::to_string(tms) + "_0.jpg");
	//cv::imwrite(path, tmpGrabImg_0);
	//threadPool.append_p(path);
}
// ���2Ӳ�������ص�����
void __stdcall ImageCallBackEx_1(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser) {
	cv::Mat tmpGrabImg_1 = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
	//std::string path = "./Image/ImageForGrab/";
	//// ��ȡʱ���
	//timeb t;
	//ftime(&t);
	//long long tms = std::move(t.time * 1000 + t.millitm);
	//path += (std::to_string(tms) + "_1.jpg");
	//cv::imwrite(path, tmpGrabImg_1);
	//threadPool.append_p(path);
}
// ���3Ӳ�������ص�����
void __stdcall ImageCallBackEx_2(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser) {
	cv::Mat tmpGrabImg_2 = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
	//std::string path = "./Image/ImageForGrab/";
	//// ��ȡʱ���
	//timeb t;
	//ftime(&t);
	//long long tms = std::move(t.time * 1000 + t.millitm);
	//path += (std::to_string(tms) + "_2.jpg");
	//cv::imwrite(path, tmpGrabImg_2);
	//threadPool.append_p(path);
}
// ���4Ӳ�������ص�����
void __stdcall ImageCallBackEx_3(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser) {
	cv::Mat tmpGrabImg_3 = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
	//std::string path = "./Image/ImageForGrab/";
	//// ��ȡʱ���
	//timeb t;
	//ftime(&t);
	//long long tms = std::move(t.time * 1000 + t.millitm);
	//path += (std::to_string(tms) + "_3.jpg");
	//cv::imwrite(path, tmpGrabImg_3);
	//threadPool.append_p(path);
}

bool CameraDrive::startGrabForExternalTrigger() const {
	for (unsigned int i = 0; i < handle.size(); i++) {
		// start grab
		if (MV_CC_StartGrabbing(handle[i]) != MV_OK)
		{
			printf("Start Grabbing fail! [%u]\n", i);
			return false;
		}
		printf("Start camera grab success! [%u]\n", i);
	}
	return true;
}

// ��ʼ���������ΪӲ������
bool CameraDrive::camerasInitInHardTriggerMode() const {
	int nRet = MV_OK;
	// Enum device
	MV_CC_DEVICE_INFO_LIST stDeviceList;
	memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
	nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
	if (MV_OK != nRet) {	// ö���ⲿ���ʧ��
		printf("Enum Devices fail! nRet [0x%x]\n", nRet);
		return false;
	}
	else {
		for (unsigned int i = 0; i < stDeviceList.nDeviceNum; i++)
		{
			printf("[device %d]:\n", i);
			MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList.pDeviceInfo[i];
			if (NULL == pDeviceInfo)
			{
				break;
			}
			PrintDeviceInfo(pDeviceInfo);
		}
	}

	for (unsigned int i = 0; i < stDeviceList.nDeviceNum; i++) {
		// Select device and create handle
		if (MV_CC_CreateHandle(&handle[i], stDeviceList.pDeviceInfo[i]) != MV_OK) {
			printf("Create Handle fail! [%u]\n", i);
			return false;
		}
		// open device
		if (MV_CC_OpenDevice(handle[i]) != MV_OK) {
			printf("Open Device fail! [%u]\n", i);
			return false;
		}
		// Detection network optimal package size(It only works for the GigE camera)
		if (stDeviceList.pDeviceInfo[i]->nTLayerType == MV_GIGE_DEVICE)
		{
			int nPacketSize = MV_CC_GetOptimalPacketSize(handle[i]);
			if (nPacketSize > 0)
			{
				if (MV_CC_SetIntValue(handle[i], "GevSCPSPacketSize", nPacketSize) != MV_OK)
				{
					printf("Warning: Set Packet Size fail! [%u]\n", i);
					return false;
				}
			}
			else
			{
				printf("Warning: Get Packet Size fail! [%u]\n", i);
				return false;
			}
		}
		// �������������Ϊ�ⲿӲ������
		// Set trigger mode as on
		if (MV_CC_SetEnumValue(handle[i], "TriggerMode", MV_TRIGGER_MODE_ON) != MV_OK)	// �ⲿ����ģʽֻ��������������
		{
			printf("Set Trigger Mode as On fail! [%u]\n", i);
			return false;
		}
		// Set trigger source from LINE0
		if (MV_CC_SetEnumValue(handle[i], "TriggerSource", MV_TRIGGER_SOURCE_LINE0) != MV_OK)	// ʹ��LINE0��Ӳ������
		{
			printf("Set Trigger Source LINE0 fail! nRet [%u]\n", i);
			return false;
		}
		// Set trigger method as RisingEdge 
		if (MV_CC_SetEnumValue(handle[i], "TriggerActivation", 3) != MV_OK)	// ʹ��LINE0��Ӳ������������Ϊ�����أ�3Ϊ�͵�ƽ��0Ϊ������
		{
			printf("Set Trigger Source LINE0 fail! nRet [%u]\n", i);
			return false;
		}
		switch (i)	// ��Բ�ͬ��������������ò�ͬ�Ļص�������������Ҫ���������޸�
		{
			// register call back function
			case 0: {
				//std::cout << "register call back function" << std::endl;
				if (MV_CC_RegisterImageCallBackEx(handle[i], ImageCallBackEx_0, handle[i]) != MV_OK)	// �ص�������������ⲿӲ�����ź�
				{
					printf("Register Image CallBack fail! nRet [0x%x], [%u]\n", nRet, i);
					return false;
				}
				break;
			}
			case 1: {
				if (MV_CC_RegisterImageCallBackEx(handle[i], ImageCallBackEx_1, handle[i]) != MV_OK)	// �ص�������������ⲿӲ�����ź�
				{
					printf("Register Image CallBack fail! nRet [0x%x], [%u]\n", nRet, i);
					return false;
				}
				break;
			}
			case 2: {
				if (MV_CC_RegisterImageCallBackEx(handle[i], ImageCallBackEx_2, handle[i]) != MV_OK)	// �ص�������������ⲿӲ�����ź�
				{
					printf("Register Image CallBack fail! nRet [0x%x], [%u]\n", nRet, i);
					return false;
				}
				break;
			}
			case 3: {
				if (MV_CC_RegisterImageCallBackEx(handle[i], ImageCallBackEx_3, handle[i]) != MV_OK)	// �ص�������������ⲿӲ�����ź�
				{
					printf("Register Image CallBack fail! nRet [0x%x], [%u]\n", nRet, i);
					return false;
				}
				break;
			}
			default:
				break;
		}
		
		// Get payload size
		MVCC_INTVALUE stParam;
		memset(&stParam, 0, sizeof(MVCC_INTVALUE));
		if (MV_CC_GetIntValue(handle[i], "PayloadSize", &stParam) != MV_OK)
		{
			printf("Get PayloadSize fail! [%u]\n", i);
			return false;
		}
		g_nPayloadSize[i] = stParam.nCurValue;
	}

	return true;
}

const int CameraDrive::getExternalCameraNum() const{
	int nRet = MV_OK;
	// Enum device
	MV_CC_DEVICE_INFO_LIST stDeviceList;
	memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
	nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
	if (MV_OK != nRet) {	// ö���ⲿ���ʧ��
		printf("Enum Devices fail! nRet [0x%x]\n", nRet);
		return -1;
	}
	else{	// �ɹ�����ⲿ���
		printf("External Cameras nums: [%u]", stDeviceList.nDeviceNum);
	}

	handle.resize(stDeviceList.nDeviceNum);
	g_nPayloadSize.resize(stDeviceList.nDeviceNum);

	return stDeviceList.nDeviceNum;
}

// print the discovered devices information to user
bool CameraDrive::PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo) const
{
	if (NULL == pstMVDevInfo)
	{
		printf("The Pointer of pstMVDevInfo is NULL!\n");
		return false;
	}
	if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE)
	{
		int nIp1 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
		int nIp2 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
		int nIp3 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
		int nIp4 = (pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

		// print current ip and user defined name
		printf("CurrentIp: %d.%d.%d.%d\n", nIp1, nIp2, nIp3, nIp4);
		printf("UserDefinedName: %s\n\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
	}
	else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE)
	{
		printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
		printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
		printf("Device Number: %d\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.nDeviceNumber);
	}
	else
	{
		printf("Not support.\n");
	}

	return true;
}

int CameraDrive::RGB2BGR(unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight) const
{
	if (NULL == pRgbData)
	{
		return MV_E_PARAMETER;
	}

	for (unsigned int j = 0; j < nHeight; j++)
	{
		for (unsigned int i = 0; i < nWidth; i++)
		{
			unsigned char red = pRgbData[j * (nWidth * 3) + i * 3];
			pRgbData[j * (nWidth * 3) + i * 3] = pRgbData[j * (nWidth * 3) + i * 3 + 2];
			pRgbData[j * (nWidth * 3) + i * 3 + 2] = red;
		}
	}

	return MV_OK;
}

// convert data stream in Mat format
bool CameraDrive::Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char * pData, cv::Mat & srcImg) const
{
	if (pstImageInfo->enPixelType == PixelType_Gvsp_Mono8)
	{
		srcImg = cv::Mat(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC1, pData);
	}
	else if (pstImageInfo->enPixelType == PixelType_Gvsp_RGB8_Packed)
	{
		RGB2BGR(pData, pstImageInfo->nWidth, pstImageInfo->nHeight);
		srcImg = cv::Mat(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC3, pData);
	}
	else
	{
		printf("unsupported pixel format\n");
		return false;
	}

	if (NULL == srcImg.data)
	{
		return false;
	}

	return true;
}
