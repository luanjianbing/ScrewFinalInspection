#pragma execution_character_set("utf-8")

#include "ScrewFinalInspection.h"
#include <QtWidgets/QApplication>
#include "common.h"

int main(int argc, char *argv[])
{
	std::cout << __cplusplus << std::endl;

    QApplication a(argc, argv);

	CameraDrive *myCameras = new CameraDrive();	// 分配空间
	const int cameraNum = myCameras->getExternalCameraNum();

	//if (cameraNum == -1) {
	//	QMessageBox::warning(NULL, "警告", "检测外部相机异常！", QMessageBox::Yes);
	//	exit(0);
	//}
	//else if (cameraNum == 0){
	//	QMessageBox::warning(NULL, "警告", "未检测到外部相机，请检查连接后重新打开软件！", QMessageBox::Yes);
	//	exit(0);
	//}
	//else {
	//	QString msg = "成功检测外部相机，数量: [" + QString::fromStdString(std::to_string(cameraNum)) + "]";
	//	QMessageBox::information(NULL, "提示", msg, QMessageBox::Yes);
	//}

	// 初始化共享内存区，当前测试默认相机只有2个
	SingleMemory::getInstance()->initSingleMemorySize(2, detectTypeNum);

	myCameras->camerasInitInHardTriggerMode();
    ScrewFinalInspection w(myCameras, cameraNum, detectTypeNum);
    w.show();
    a.exec();

	std::cout << "delete cameras" << std::endl;
	delete myCameras;
	return 1;
}
