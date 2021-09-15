#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ScrewFinalInspection.h"

#include <QMessageBox>

#include "CameraDrive.h"
#include "SingleMemory.h"

#include <QTimer>
class ScrewFinalInspection : public QMainWindow
{
    Q_OBJECT

public:
    ScrewFinalInspection(CameraDrive *myCameras, const int &myCamerasNum, const int &detectTypeNum, QWidget *parent = Q_NULLPTR);
	~ScrewFinalInspection();

private:
    Ui::ScrewFinalInspectionClass ui;

	CameraDrive *myCameras = nullptr;
	const int myCamerasNum = 0;
	const int detectTypeNum = 0;

	QTimer *mTimer = nullptr;
private slots:
	void onActionTestCameraPosition();	// 响应'测试相机位置'动作
	void onActionStartRun();	// 响应'开始运行'动作

	void onButtonSoftTrigger();

	void update();
};
