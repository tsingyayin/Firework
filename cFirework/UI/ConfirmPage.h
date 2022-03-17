#pragma once
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include "../langcontrol.h"
#include "../global_value.h"
#include "../loadsettings.h"
#include "CEWidgets.h"
#include "../Func/ImageAna.h"
#pragma execution_character_set("utf-8")

class uConfirmPage :public QFrame
{
	Q_OBJECT
signals:
	void closed(QString);	
public:
	QLabel* PreviewLabel;
	QLabel* TitleLabel;
	QLabel* HueLabel;
	QLabel* SaturationLabel;
	QLabel* LightnessLabel;
	QTimer* StartTimer;
	QPushButton* StartButton;
	QComboBox* LorWSelectBox;
	QLineEdit* MpPEdit;
	FlattenThread* Thread;
	QLabel* ProgressLabel;
	QLabel* ProgressBackLabel;
	QLabel* ProgressText;
	float ProgressFloat = 0;
	bool InAna = FALSE;
	uConfirmPage(QWidget* parent = Q_NULLPTR) {
		this->setParent(parent);
		this->setWindowTitle("预览当前画面识别结果");
		this->setMinimumSize(1366, 768);
		PreviewLabel = new QLabel(this);
		PreviewLabel->setText("此处无图像时，程序正在运算");
		PreviewLabel->setAlignment(Qt::AlignCenter);
		TitleLabel = new QLabel(this);
		TitleLabel->setText("  预览");
		PreviewLabel->setStyleSheet("QLabel{background-color:#000000;color:#FFFFFF;font-size:" + QString::number((int)(height() * 0.05)) + "px;font-family:'Microsoft YaHei';}");
		StartTimer = new QTimer(this);
		connect(StartTimer, SIGNAL(timeout()), this, SLOT(loadImage()));
		StartTimer->setSingleShot(TRUE);
		StartTimer->start(500);
		StartButton = new QPushButton(this);
		StartButton->setText("开始运算");
		connect(StartButton, SIGNAL(clicked()), this, SLOT(anaVideo()));
		
		ProgressBackLabel = new QLabel(this);
		ProgressLabel = new QLabel(this);
		ProgressText = new QLabel(this);
		ProgressText->setText("正在等待运算");
		ProgressText->setAlignment(Qt::AlignCenter);

		LorWSelectBox = new QComboBox(this);
		LorWSelectBox->addItem("指定上图火焰高度(单位:米)", 0);
		LorWSelectBox->addItem("指定上图火焰宽度(单位:米)", 1);
		LorWSelectBox->setCurrentIndex(0);
		MpPEdit = new QLineEdit(this);
		MpPEdit->setText("10");
		
	}

public slots:
	void anaVideo() {
		QString Num = MpPEdit->text();
		QString NumO;
		for (int i = 0; i < Num.length(); i++) {
			if (Num[i] == "0" || Num[i] == "1" || Num[i] == "2" || Num[i] == "3" || Num[i] == "4" ||
				Num[i] == "5" || Num[i] == "6" || Num[i] == "7" || Num[i] == "8" || Num[i] == "9" || Num[i] == ".")
			{
				NumO += Num[i];
			}
		}
		if (NumO.toFloat() > 0) {
			
			ProgressText->setText("初始化");
			if (LorWSelectBox->currentIndex() == 0) {
				GlobalValue::MpP = NumO.toFloat() / GlobalValue::CurrentRec[1];
			}
			else {
				GlobalValue::MpP = NumO.toFloat() / GlobalValue::CurrentRec[0];
			}
			Thread = new FlattenThread();
			connect(Thread, SIGNAL(isEnd()), this, SLOT(afterAna()));
			connect(Thread, SIGNAL(progressFloat(QString, float)), this, SLOT(updateProgress(QString, float)));
			Thread->start();
			InAna = TRUE;
			ProgressLabel->setGeometry(QRect(0, 0, 0, 0));
		}
		else {
			QMessageBox msgBox;
			msgBox.setWindowTitle("不接受的数值");
			msgBox.setText("指定的度量数值："+Num+"\n是无效数值。");
			msgBox.exec();
		}
	}
	void updateProgress(QString CurrentStep, float AnaFloat)
	{
		ProgressFloat = AnaFloat;
		ProgressLabel->setGeometry(QRect(width() * 0.1, height() * 0.7, width() * 0.8 * ProgressFloat, height() * 0.03));
		ProgressText->setText(CurrentStep + QString::number(AnaFloat * 100, 'f', 1)+"%");
	}
	void closeEvent(QCloseEvent* event = Q_NULLPTR) {
		if (InAna) {
			QMessageBox msgBox;
			msgBox.setWindowTitle("禁止的操作");
			msgBox.setText("在后台运算时，无法关闭该窗口。");
			msgBox.exec();
			event->ignore();
		}
	}
	void afterAna() {
		InAna = FALSE;
		Thread->disconnect();
		Thread->wait();
		Thread->deleteLater();
		ProgressText->setText("完毕！");
	}
	void resizeEvent(QResizeEvent* event){
		MpPEdit->setStyleSheet("QLineEdit{background-color:#3C3C3C;border:1px solid #4A4A4A;border-radius:5px}");
		MpPEdit->setGeometry(QRect(width() * 0.51, height() * 0.62, width() * 0.3, height() * 0.06));
		LorWSelectBox->setGeometry(QRect(width() * 0.2, height() * 0.62, width() * 0.29, height() * 0.06));
		ProgressLabel->setGeometry(QRect(width() * 0.1, height() * 0.7, width() * 0.8 * ProgressFloat, height() * 0.03));
		ProgressBackLabel->setGeometry(QRect(width() * 0.1, height() * 0.7, width() * 0.8, height() * 0.03));
		ProgressLabel->setStyleSheet("QLabel{border-radius:5px;background-color:qlineargradient(x1:0, y1:0, x2:1, y2:0,stop:0 #022CAD, stop:1 #21DE24);}");
		ProgressBackLabel->setStyleSheet("QLabel{background-color:#111111;border-radius:5px;}");
		ProgressText->setStyleSheet("QWidget{background-color:#333333;color:#FFFFFF;font-size:" + QString::number((int)(height() * 0.03)) + "px;font-family:'Microsoft YaHei';}");
		ProgressText->setGeometry(QRect(width() * 0.2, height() * 0.75, width() * 0.6, height() * 0.03));
		PreviewLabel->setGeometry(QRect(width() / 2 - height() * 0.5 / 9 * 16 / 2, height() * 0.11, height() * 0.5 / 9 * 16, height() * 0.5));
		this->setStyleSheet("QWidget{background-color:#333333;color:#FFFFFF;}");
		TitleLabel->setGeometry(QRect(width() * 0, height() * 0, width() * 1, height() * 0.1));
		TitleLabel->setStyleSheet("QWidget{background-color:#222222;color:#FFFFFF;font-size:" + QString::number((int)(height() * 0.04)) + "px;font-family:'Microsoft YaHei'}");
		StartButton->setGeometry(QRect(width() * 0.45, height() * 0.85, width() * 0.1, height() * 0.08));
		StartButton->setStyleSheet("\
				QPushButton\
					{border:2px solid #1473E6;border-radius:10px;font-size:" + QString::number((int)(height() * 0.03)) + "px;background-color:#555555;font-family:'Microsoft YaHei';}\
				QPushButton:hover\
					{border:2px solid #1473E6;border-radius:10px;font-size:" + QString::number((int)(height() * 0.03)) + "px;background-color:rgba(0,0,0,0);font-family:'Microsoft YaHei';}\
				QPushButton:pressed\
					{border:2px solid #1473E6;border-radius:10px;font-size:" + QString::number((int)(height() * 0.03)) + "px;background-color:#333333;font-family:'Microsoft YaHei';}");
		LorWSelectBox->setStyleSheet("\
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
                    border-radius:" + QString::number((int)(width() * 0.008)) + "px;\
                }\
				QComboBox QAbstractItemView::item {min-height:" + QString::number((int)(height() * 0.05)) + "px;}\
                QComboBox QScrollBar::handle:vertical:hover{\
                    background-color:rgba(200,200,200,1);\
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
	}

	bool loadImage() {
		PreviewLabel->setStyleSheet("QLabel{background-color:#000000;color:#FFFFFF;font-size:" + QString::number((int)(height() * 0.05)) + "px;font-family:'Microsoft YaHei';}");
		PreviewLabel->setText("正在生成预览");
		PreviewLabel->repaint();
		if (GlobalValue::RectangleGeo[0] == 0 && GlobalValue::RectangleGeo[1] == 0) {
			PreviewLabel->setText("火焰特征区域标定无效，无法生成预览");
			PreviewLabel->repaint();
			return FALSE;
		}
		if (GlobalValue::RoundGeo[0] == 0 && GlobalValue::RoundGeo[1] == 0) {
			PreviewLabel->setText("火焰根基点标定无效，无法生成预览");
			PreviewLabel->repaint();
			return FALSE;
		}
		if (GlobalValue::CurrentSelectFrame < 0) {
			PreviewLabel->setText("目标帧选定无效，请确认您选定正确");
			PreviewLabel->repaint();
			return FALSE;
		}
		ImageAnalysis::getColorListOf("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "POS" + QString::number(GlobalValue::CurrentSelectFrame) + ".jpg");
		ImageAnalysis::flattenImage("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "POS" + QString::number(GlobalValue::CurrentSelectFrame) + ".jpg");
		ImageAnalysis::giveFlameTrack("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "POS" + QString::number(GlobalValue::CurrentSelectFrame) + ".jpg", "./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "CAC" + QString::number(GlobalValue::CurrentSelectFrame) + ".jpg");
		PreviewLabel->setText("");
		PreviewLabel->setStyleSheet("QLabel{background-color:#000000;color:#FFFFFF;font-size:" + QString::number((int)(height() * 0.05)) + "px;font-family:'Microsoft YaHei';border-image:url('./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "FNL" + QString::number(GlobalValue::CurrentSelectFrame) + ".jpg');}");
	}
};