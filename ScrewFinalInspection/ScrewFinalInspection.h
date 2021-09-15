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
	void onActionTestCameraPosition();	// ��Ӧ'�������λ��'����
	void onActionStartRun();	// ��Ӧ'��ʼ����'����

	void onButtonSoftTrigger();

	void update();
};
