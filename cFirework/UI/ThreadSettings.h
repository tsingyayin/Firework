#pragma once
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "../global_value.h"
#include <windows.h>
#pragma execution_character_set("utf-8")

class ThreadSettingsPage :public QWidget
{
	Q_OBJECT
signals:
	void isClosed();
public:
	QLabel* ProjectThread_Label;
	QComboBox* ProjectThread_Box;
	QGridLayout* CurrentLayout;
	QPushButton* YesButton;
	SYSTEM_INFO SysInfo;
	int Processors;
	ThreadSettingsPage() {
		this->setWindowTitle("多线程选项");
		this->setFixedSize(550, 250);
		this->setWindowModality(Qt::ApplicationModal);
		ProjectThread_Label = new QLabel(this);
		ProjectThread_Box = new QComboBox(this);
		CurrentLayout = new QGridLayout(this);
		this->setLayout(CurrentLayout);
		YesButton = new QPushButton(this);
		YesButton->setText("保存并退出");
		ProjectThread_Label->setText("更改最大线程数");
		GetSystemInfo(&SysInfo);
		Processors = SysInfo.dwNumberOfProcessors;
		for (int i = 1; i <= Processors; i++) {
			ProjectThread_Box->addItem(QString::number(i), i - 1);
		}
		ProjectThread_Box->setCurrentIndex(GlobalValue::MaxAliveThread - 1);
		CurrentLayout->addWidget(ProjectThread_Label, 0, 1, 1, 1);
		CurrentLayout->addWidget(ProjectThread_Box, 0, 3, 1, 1);
		CurrentLayout->addWidget(YesButton, 1, 2, 1, 1);
		connect(YesButton, SIGNAL(clicked()), this, SLOT(saveAndExit()));
	}
public slots:
	void saveAndExit() {
		this->hide();
		GlobalValue::MaxAliveThread = ProjectThread_Box->currentIndex() + 1;
		emit isClosed();
		this->close();
	}
	void resizeEvent(QResizeEvent* event = Q_NULLPTR) {
		this->setStyleSheet("\
			QWidget{\
				background-color:#444444;\
				color:#FFFFFF;\
				font-size:" + QString::number((int)(height() * 0.08)) + "px;\
				font-family:'Microsoft YaHei';\
			}");
		ProjectThread_Box->setStyleSheet("\
			QComboBox\
				{background-color:#3C3C3C;border:1px solid #4A4A4A;border-radius:5px}\
			QComboBox::drop-down\
				{background-color:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop: 0 #999999,stop: 1.0 #666666);border:1px solid #4A4A4A;border-radius:5px}\
			QComboBox QScrollBar:vertical{\
                    background-color:rgba(0,0,0,0);\
                    margin:0px,0px,0px,0px;\
                    padding-top:0px;\
                    padding-bottom:0px;\
                }\
                QComboBox QScrollBar::handle:vertical{\
                    background-color:rgba(255,255,255,1);\
                    border-radius:" + QString::number((int)(width() * 0.08)) + "px;\
                }\
                QComboBox QScrollBar::handle:vertical:hover{\
                    background-color:rgba(200,200,200,1);\
                    border-radius:" + QString::number((int)(width() * 0.08)) + "px;\
                }\
                QComboBox QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{\
                    background-color:rgba(0,0,0,0);\
                }\
                QComboBox QScrollBar::add-line:vertical{\
                    height:0px;\
                    width:0px;\
                    subcontrol-position:bottom;\
                }\
                QComboBox QScrollBar::sub-line:vertical{\
                    height:0px;\
                    width:0px;\
                    subcontrol-position:top;\
                    }");
		YesButton->setStyleSheet("\
				QPushButton\
					{border:2px solid #1473E6;border-radius:10px;background-color:#555555;}\
				QPushButton:hover\
					{border:2px solid #1473E6;border-radius:10px;background-color:rgba(0,0,0,0);}\
				QPushButton:pressed\
					{border:2px solid #1473E6;border-radius:10px;background-color:#333333;}");
	}
	void closeEvent(QCloseEvent* event = Q_NULLPTR) {
		this->disconnect();
		this->deleteLater();
	}
};