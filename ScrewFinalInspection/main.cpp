#pragma execution_character_set("utf-8")

#include "ScrewFinalInspection.h"
#include <QtWidgets/QApplication>
#include "common.h"

int main(int argc, char *argv[])
{
	std::cout << __cplusplus << std::endl;

    QApplication a(argc, argv);

	CameraDrive *myCameras = new CameraDrive();	// ����ռ�
	const int cameraNum = myCameras->getExternalCameraNum();

	//if (cameraNum == -1) {
	//	QMessageBox::warning(NULL, "����", "����ⲿ����쳣��", QMessageBox::Yes);
	//	exit(0);
	//}
	//else if (cameraNum == 0){
	//	QMessageBox::warning(NULL, "����", "δ��⵽�ⲿ������������Ӻ����´������", QMessageBox::Yes);
	//	exit(0);
	//}
	//else {
	//	QString msg = "�ɹ�����ⲿ���������: [" + QString::fromStdString(std::to_string(cameraNum)) + "]";
	//	QMessageBox::information(NULL, "��ʾ", msg, QMessageBox::Yes);
	//}

	// ��ʼ�������ڴ�������ǰ����Ĭ�����ֻ��2��
	SingleMemory::getInstance()->initSingleMemorySize(2, detectTypeNum);

	myCameras->camerasInitInHardTriggerMode();
    ScrewFinalInspection w(myCameras, cameraNum, detectTypeNum);
    w.show();
    a.exec();

	std::cout << "delete cameras" << std::endl;
	delete myCameras;
	return 1;
}
