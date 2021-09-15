#include "ScrewFinalInspection.h"

ScrewFinalInspection::ScrewFinalInspection(CameraDrive *myCameras, const int &myCamerasNum, 
	const int &detectTypeNum, QWidget *parent)
	: myCameras(myCameras), myCamerasNum(myCamerasNum), detectTypeNum(detectTypeNum), QMainWindow(parent)
{
	ui.setupUi(this);

	

	// actionTestCameraPosition
	connect(ui.actionTestCameraPosition, SIGNAL(triggered()), this, SLOT(onActionTestCameraPosition()));

	// actionTestCameraPosition
	connect(ui.actionStartRun, SIGNAL(triggered()), this, SLOT(onActionStartRun()));

	// buttonSoftTrigger
	connect(ui.buttonSoftTrigger, SIGNAL(released()), this, SLOT(onButtonSoftTrigger()));

	if (mTimer == nullptr) {
		mTimer = new QTimer();
		mTimer->setInterval(50);
		mTimer->start();

		connect(mTimer, SIGNAL(timeout()), this, SLOT(update()));
	}
}

void ScrewFinalInspection::onActionTestCameraPosition() {
	myCameras->startGrabForExternalTrigger();
}

void ScrewFinalInspection::onActionStartRun() {
	//ui.labelDisplayRealTime_0->setPixmap(QPixmap("./dst.jpg").scaled(ui.labelDisplayRealTime_0->width(), ui.labelDisplayRealTime_0->height(), Qt::KeepAspectRatio));
	//ui.labelDisplayRealTime_1->setPixmap(QPixmap("./dst_1.jpg").scaled(ui.labelDisplayRealTime_1->width(), ui.labelDisplayRealTime_1->height(), Qt::KeepAspectRatio));
	//ui.labelDisplayRealTime_2->setPixmap(QPixmap("./test1.jpg").scaled(ui.labelDisplayRealTime_2->width(), ui.labelDisplayRealTime_2->height(), Qt::KeepAspectRatio));
	//ui.labelDisplayRealTime_3->setPixmap(QPixmap("./test2.jpg").scaled(ui.labelDisplayRealTime_3->width(), ui.labelDisplayRealTime_3->height(), Qt::KeepAspectRatio));
}

void ScrewFinalInspection::onButtonSoftTrigger() {
	cv::Mat test1 = cv::imread("./dst.jpg");
	cv::cvtColor(test1, test1, CV_BGR2GRAY);

	int camNum = 2;

	// 探索编号
	int i = 0;
	while (SingleMemory::getInstance()->LRUCacheCameraAndMatContains(0 + camNum * i)) {
		std::cout << 0 + camNum * i << std::endl;
		i += 1;
	}

	SingleMemory::getInstance()->putLRUCacheCameraIndexAndMat(0 + camNum * i, test1);
	SingleMemory::getInstance()->reSetMapStatus(0 + camNum * i, 5);

	std::cout << " put: " << (0 + camNum * i) << std::endl;

	// 添加任务测试
	threadPool.append_p(std::to_string(0 + camNum * i) + ' ' + std::to_string(FLAW1));
	threadPool.append_p(std::to_string(0 + camNum * i) + ' ' + std::to_string(FLAW2));
	threadPool.append_p(std::to_string(0 + camNum * i) + ' ' + std::to_string(FLAW3));
	threadPool.append_p(std::to_string(0 + camNum * i) + ' ' + std::to_string(FLAW4));
	threadPool.append_p(std::to_string(0 + camNum * i) + ' ' + std::to_string(MEASURE2));

	cv::Mat test2 = cv::imread("./test2.jpg");
	cv::cvtColor(test2, test2, CV_BGR2GRAY);

	// 探索编号
	i = 0;
	while (SingleMemory::getInstance()->LRUCacheCameraAndMatContains(1 + camNum * i)) {
		std::cout << 1 + camNum * i << std::endl;
		i += 1;
	}

	SingleMemory::getInstance()->putLRUCacheCameraIndexAndMat(1 + camNum * i, test2);
	SingleMemory::getInstance()->reSetMapStatus(1 + camNum * i, 5);

	std::cout << " put: " << (1 + camNum * i) << std::endl;

	threadPool.append_p(std::to_string(1 + camNum * i) + ' ' + std::to_string(FLAW5));
	threadPool.append_p(std::to_string(1 + camNum * i) + ' ' + std::to_string(FLAW6));
	threadPool.append_p(std::to_string(1 + camNum * i) + ' ' + std::to_string(FLAW7));
	threadPool.append_p(std::to_string(1 + camNum * i) + ' ' + std::to_string(FLAW8));
	threadPool.append_p(std::to_string(1 + camNum * i) + ' ' + std::to_string(MEASURE1));

	
}

void ScrewFinalInspection::update() {
	int cam = SingleMemory::getInstance()->getFinishedCamera();
	//std::cout << cam << std::endl;
	if (cam != -1) {
		int trueCameraNum = cam % detectTypeNum;
		if (trueCameraNum == 0) {
			ui.labelDisplayRealTime_0->clear();
			cv::Mat orgMat = SingleMemory::getInstance()->getLRUCacheMatByCameraIndex(cam);
			QImage singleQImage(orgMat.data, orgMat.cols, orgMat.rows, orgMat.step, QImage::Format_Indexed8);
			ui.labelDisplayRealTime_0->setPixmap(QPixmap::fromImage(singleQImage).scaled(ui.labelDisplayRealTime_0->width(), ui.labelDisplayRealTime_0->height(), Qt::KeepAspectRatio));
		}
		else if (trueCameraNum == 1) {
			ui.labelDisplayRealTime_1->clear();
			cv::Mat orgMat = SingleMemory::getInstance()->getLRUCacheMatByCameraIndex(cam);
			QImage singleQImage(orgMat.data, orgMat.cols, orgMat.rows, orgMat.step, QImage::Format_Indexed8);
			ui.labelDisplayRealTime_1->setPixmap(QPixmap::fromImage(singleQImage).scaled(ui.labelDisplayRealTime_1->width(), ui.labelDisplayRealTime_1->height(), Qt::KeepAspectRatio));
		}

		std::vector<int> thisCameraResult = SingleMemory::getInstance()->getCurCameraDetectResult(cam);

		int iRow = ui.tableWidget->rowCount();
		ui.tableWidget->setRowCount(iRow + 1);//总行数增加1
		
		for (int i = 0; i < thisCameraResult.size(); ++i) {
			ui.tableWidget->setItem(iRow, i, new QTableWidgetItem(std::to_string(thisCameraResult[i]).c_str()));
		}
	}
}

ScrewFinalInspection::~ScrewFinalInspection() {
	if (mTimer != nullptr && mTimer->isActive()) {
		mTimer->stop();
		delete mTimer;
	}
}