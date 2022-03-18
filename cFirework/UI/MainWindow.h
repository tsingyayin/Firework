#pragma once
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtMultimedia>
#include "Widget.h"
#include "SelectVideos.h"
#include "ConfirmPage.h"
#include "../Func/FrameDelay.h"
#include "CEWidgets.h"
#include "../Func/ProjectCES.h"
#include "../global_value.h"
#include "../loadsettings.h"
#include "CEInfoPage.h"
#include "ThreadSettings.h"

#pragma execution_character_set("utf-8")

class uProgressWidget :public QFrame
{
	Q_OBJECT
signals:
	void countChanged(int);
	void valueChanged(qint64);
public:
	CE::CEMultiHorizontalScrollBar* RawVideo;
	uProgressWidget(QWidget* parent = Q_NULLPTR) {
		this->setParent(parent);
		this->setObjectName("ProgressFrame");

		RawVideo = new CE::CEMultiHorizontalScrollBar(this);
		RawVideo->setObjectName("Frame");
		RawVideo->setStyleSheet("QFrame#Frame{background-color:#555555;border:2px solid #222222;border-radius:5px}\
														QLabel#INSIDERCOVER{border-right:2px solid #222222;}");
		RawVideo->setScaleStyleSheet("QLabel#INSIDERSCALE{background-color:#FFFFFF;border-radius:5px;}");
		
	
		setMaximum(1000);
		connect(RawVideo, SIGNAL(countChanged(int)), this, SIGNAL(countChanged(int)));
		connect(RawVideo, SIGNAL(valueChanged(qint64)), this, SLOT(distributeValue(qint64)));
		connect(RawVideo, SIGNAL(valueChanged(qint64)), this, SIGNAL(valueChanged(qint64)));

	}
	void setMinimum(qint64 min) {
		RawVideo->setMinimum(min);

	}
	void setMaximum(qint64 max) {
		RawVideo->setMaximum(max);

	}
	void resizeEvent(QResizeEvent* event = Q_NULLPTR) {
		this->setStyleSheet("QFrame#ProgressFrame{background-color:#333333;}");
		RawVideo->setGeometry(QRect(width() * 0.01, height() * 0.1, width() * 0.98, height() * 0.18));


	}
public slots:
	void distributeValue(qint64 value) {
		RawVideo->setValue(value);


	}
	void distributeValueWithSignal(qint64 value) {
		RawVideo->setValueWithSignal(value);

	}
	qint64 value() {
		return RawVideo->value();
	}
	void setRawImage(int value,QString ImageDir) {
		RawVideo->loadImage(value, ImageDir);
	}

};

class uINPlayerWidget :public QFrame
{
	Q_OBJECT
signals:
	void geoInfoChanged();
public:
	enum class EditState
	{
		Unknown = 0,
		PlayMode = 1,
		EditMode = 2,
	};
	enum class ToolState
	{
		Unknown = 0,
		Disable = 1,
		Rectangle = 2,
		Round = 3,
	};
	QMediaPlayer* MediaPlayer;
	QVideoWidget* VideoWidget;
	QLabel* EditWidget;
	QMediaPlaylist* PlayList;
	QPushButton* PlayControlButton;
	QPushButton* NextFrameButton;
	QPushButton* BackFrameButton;
	CE::CEMultiButton* PlayModeButton;
	CE::CEMultiButton* EditModeButton;
	CE::CEButtonGroup* ButtonGroup;
	QPushButton* ToolFrameButton;
	QPushButton* ToolRootButton;
	EditState CurrentState = EditState::PlayMode;
	ToolState ToolsState = ToolState::Disable;
	QLabel* RectangleLabel;
	QLabel* RoundLabel;
	QLabel* ColorDistributionLabel;
	QPushButton* CalculateButton;
	uConfirmPage* CalculatePage;
	uINPlayerWidget(QWidget* parent = Q_NULLPTR) {
		this->setParent(parent);
		VideoWidget = new QVideoWidget(this);
		EditWidget = new QLabel(this);
		EditWidget->setObjectName("EditWidget");

		MediaPlayer = new QMediaPlayer(this);
		PlayList = new QMediaPlaylist(this);
		PlayControlButton = new QPushButton(this);
		connect(PlayControlButton, SIGNAL(clicked()), this, SLOT(playerStateChange()));
		connect(MediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(_playerStateChange(QMediaPlayer::State)));
		
		
		NextFrameButton = new QPushButton(this);
		BackFrameButton = new QPushButton(this);
		PlayControlButton->setStyleSheet("\
			QPushButton{border-image:url(':IRC/Icon/PlayButton');background-color:#222222;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:hover{border-image:url(':IRC/Icon/PlayButton');background-color:#555555;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:pressed{border-image:url(':IRC/Icon/PlayButton');background-color:#888888;border:2px solid #FFFFFF;border-radius:5px}");
		NextFrameButton->setStyleSheet("\
			QPushButton{border-image:url(':IRC/Icon/NFButton');background-color:#222222;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:hover{border-image:url(':IRC/Icon/NFButton');background-color:#555555;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:pressed{border-image:url(':IRC/Icon/NFButton');background-color:#888888;border:2px solid #FFFFFF;border-radius:5px}");
		BackFrameButton->setStyleSheet("\
			QPushButton{border-image:url(':IRC/Icon/BFButton');background-color:#222222;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:hover{border-image:url(':IRC/Icon/BFButton');background-color:#555555;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:pressed{border-image:url(':IRC/Icon/BFButton');background-color:#888888;border:2px solid #FFFFFF;border-radius:5px}");
		PlayModeButton = new CE::CEMultiButton(CE::TILayout::HIT, this, true);
		EditModeButton = new CE::CEMultiButton(CE::TILayout::HIT, this, true);
		ButtonGroup = new CE::CEButtonGroup(this);
		ButtonGroup->addCEMultiButton(PlayModeButton);
		ButtonGroup->addCEMultiButton(EditModeButton);
		PlayModeButton->setText("回放模式");
		EditModeButton->setText("编辑模式");
		PlayModeButton->setCentralImage(":/IRC/Icon/PlayMode.png");
		EditModeButton->setCentralImage(":/IRC/Icon/EditMode.png");
		PlayModeButton->setTextAlignment(Qt::AlignTop);
		EditModeButton->setTextAlignment(Qt::AlignTop);
		PlayModeButton->setFontPercentage(0.8);
		EditModeButton->setFontPercentage(0.8);
		PlayModeButton->setObjectName("PlayModeButton");
		PlayModeButton->setNormalStyleSheet("\
				QFrame#PlayModeButton\
					{border:2px solid #4A4A4A;border-radius:10px;background-color:#666666;}\
				QFrame#PlayModeButton:hover\
					{border:2px solid #4A4A4A;border-radius:10px;background-color:#444444;}");
		PlayModeButton->setPressStyleSheet("\
				QFrame#PlayModeButton\
					{border:2px solid #4A4A4A;border-radius:10px;background-color:rgba(0,0,0,0);}");
		EditModeButton->setObjectName("EditModeButton");
		EditModeButton->setNormalStyleSheet("\
				QFrame#EditModeButton\
					{border:2px solid #4A4A4A;border-radius:10px;background-color:#666666;}\
				QFrame#EditModeButton:hover\
					{border:2px solid #4A4A4A;border-radius:10px;background-color:#444444;}");
		EditModeButton->setPressStyleSheet("\
				QFrame#EditModeButton\
					{border:2px solid #4A4A4A;border-radius:10px;background-color:rgba(0,0,0,0);}");
		PlayModeButton->setTextStyleSheet("QLabel{font-size:__AUTO__;color:#FFFFFF;font-family:'Microsoft YaHei';background-color:rgba(0,0,0,0);}");
		EditModeButton->setTextStyleSheet("QLabel{font-size:__AUTO__;color:#FFFFFF;font-family:'Microsoft YaHei';background-color:rgba(0,0,0,0);}");
		PlayModeButton->click();
		MediaPlayer->setPlaylist(PlayList);
		MediaPlayer->setVideoOutput(VideoWidget);
		this->setStyleSheet("QFrame{background:#222222};");
		QPalette BGPalette = VideoWidget->palette();
		BGPalette.setColor(QPalette::Background, Qt::black);
		VideoWidget->setAutoFillBackground(true);
		VideoWidget->setPalette(BGPalette);
		VideoWidget->raise();
		connect(ButtonGroup, SIGNAL(refreshed()), this, SLOT(changeState()));

		ToolFrameButton = new QPushButton(this);
		ToolRootButton = new QPushButton(this);

		connect(ToolFrameButton, SIGNAL(clicked()), this, SLOT(_changeCuresor()));
		connect(ToolRootButton, SIGNAL(clicked()), this, SLOT(_changeCuresor()));
		ToolFrameButton->setStyleSheet("\
			QPushButton{border-image:url(':IRC/Icon/FrameTool');background-color:#222222;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:hover{border-image:url(':IRC/Icon/FrameTool');background-color:#555555;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:pressed{border-image:url(':IRC/Icon/FrameTool');background-color:#888888;border:2px solid #FFFFFF;border-radius:5px}");
		ToolRootButton->setStyleSheet("\
			QPushButton{border-image:url(':IRC/Icon/RootTool');background-color:#222222;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:hover{border-image:url(':IRC/Icon/RootTool');background-color:#555555;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:pressed{border-image:url(':IRC/Icon/RootTool');background-color:#888888;border:2px solid #FFFFFF;border-radius:5px}");
		ToolFrameButton->hide();
		ToolRootButton->hide();

		RectangleLabel = new QLabel(this);
		RoundLabel = new QLabel(this);
		RectangleLabel->setObjectName("Rectangle");
		RoundLabel->setObjectName("Round");
		RectangleLabel->setStyleSheet("QLabel#Rectangle{border:3px solid green;background-color:rgba(0,0,0,0);}");
		RoundLabel->setStyleSheet("QLabel#Round{border:3px solid red;border-radius:10px;background-color:rgba(0,0,0,0);}");
		RoundLabel->setFixedSize(20, 20);
		RectangleLabel->hide();
		RoundLabel->hide();

		CalculateButton = new QPushButton(this);
		CalculateButton->setText("运算");
		CalculateButton->hide();
		connect(CalculateButton, SIGNAL(clicked()), this, SLOT(launchCalculate()));
	}
public slots:
	void launchCalculate() {
		mouseReleaseEvent();
		CalculatePage = new uConfirmPage();
		CalculatePage->setWindowModality(Qt::WindowModality::ApplicationModal);
		connect(CalculatePage, SIGNAL(closed(QString)), this, SLOT(afterCalculate(QString)));
		CalculatePage->show();
	}
	void afterCalculate(QString CalculateState) {
		CalculatePage->disconnect();
		CalculatePage->deleteLater();
	}
	void loadGeometryInfo(float Rx, float Ry, float Rw, float Rh, float Ox, float Oy) {
		RectangleLabel->setGeometry(QRect(Rx * width(), Ry * height(), Rw * width(), Rh * height()));
		RoundLabel->move(Ox * width(), Oy * height());
	}
	void changeState(void) {
		QString SelectWhich =ButtonGroup->currentString();
		if (SelectWhich == "回放模式") {
			CurrentState = EditState::PlayMode;
			PlayControlButton->show();
			CalculateButton->hide();
			ToolFrameButton->hide();
			ToolRootButton->hide();
			RectangleLabel->hide();
			RoundLabel->hide();
			VideoWidget->raise();
			QCursor Native;
			this->setCursor(Native);
		}
		else {
			CurrentState = EditState::EditMode;
			PlayControlButton->hide();
			CalculateButton->show();
			ToolFrameButton->show();
			ToolRootButton->show();
			EditWidget->raise();
			RectangleLabel->raise();
			RoundLabel->raise();
			if (RectangleLabel->pos().x() > EditWidget->pos().x() && RectangleLabel->pos().y() > EditWidget->pos().y()) {
				RectangleLabel->show();
			}
			if (RoundLabel->pos().x() > EditWidget->pos().x() && RoundLabel->pos().y() > EditWidget->pos().y()) {
				RoundLabel->show();
			}
			changeCursor();
			exportCurrentFrame(MediaPlayer->position());
			if (MediaPlayer->state() == QMediaPlayer::State::PlayingState) {
				PlayControlButton->click();
				//MediaPlayer->stop();
			}
		}
	}

	void _playerStateChange(QMediaPlayer::State States) {
		//playerStateChange();
	}
	void playerStateChange(void) {
		switch (MediaPlayer->state()) {
		case QMediaPlayer::State::PlayingState:
			PlayControlButton->setStyleSheet("\
			QPushButton{border-image:url(':IRC/Icon/PlayButton');background-color:#222222;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:hover{border-image:url(':IRC/Icon/PlayButton');background-color:#555555;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:pressed{border-image:url(':IRC/Icon/PlayButton');background-color:#888888;border:2px solid #FFFFFF;border-radius:5px}");
			MediaPlayer->pause();
			break;
		case QMediaPlayer::State::PausedState:
			PlayControlButton->setStyleSheet("\
			QPushButton{border-image:url(':IRC/Icon/StopButton');background-color:#222222;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:hover{border-image:url(':IRC/Icon/StopButton');background-color:#555555;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:pressed{border-image:url(':IRC/Icon/StopButton');background-color:#888888;border:2px solid #FFFFFF;border-radius:5px}");
			MediaPlayer->play();
			break;
		case QMediaPlayer::State::StoppedState:
			PlayControlButton->setStyleSheet("\
			QPushButton{border-image:url(':IRC/Icon/StopButton');background-color:#222222;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:hover{border-image:url(':IRC/Icon/StopButton');background-color:#555555;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:pressed{border-image:url(':IRC/Icon/StopButton');background-color:#888888;border:2px solid #FFFFFF;border-radius:5px}");
			MediaPlayer->play();
			break;
		}
	}
	void setPosition(qint64 Position) {
		MediaPlayer->setPosition(Position);
		if (CurrentState == EditState::EditMode) {
			exportCurrentFrame(Position);
		}
	}
	void _changeCuresor() {
		QPushButton* Sender = static_cast<QPushButton*>(this->sender());
		if (Sender == ToolFrameButton) {
			ToolsState = ToolState::Rectangle;
		}
		else if (Sender == ToolRootButton) {
			ToolsState = ToolState::Round;
		}
		changeCursor();
	}
	void changeCursor() {
		QCursor Frame(QPixmap(":/IRC/Icon/FrameCursor.png"));
		QCursor Root(QPixmap(":/IRC/Icon/RootCursor.png"));
		switch (ToolsState) {
		case ToolState::Rectangle:
			this->setCursor(Frame);
			break;
		case ToolState::Round:
			this->setCursor(Root);
			break;
		}
		
	}
	void exportCurrentFrame(qint64 Position) {
		try {
			GlobalValue::CurrentSelectFrame = (int)((double)Position / GlobalValue::CurrentDuration * GlobalValue::CurrentFrame);
			qDebug() << GlobalValue::CurrentSelectFrame;
			QFile SelectFile;
			SelectFile.setFileName("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "POS" + QString::number(GlobalValue::CurrentSelectFrame) + ".jpg");
			if (!SelectFile.exists()) {
				cv::VideoCapture VideoObject;
				cv::Mat Frame;
				VideoObject.open(GlobalValue::CurrentVideo.toStdString());
				VideoObject.set(cv::CAP_PROP_POS_FRAMES, GlobalValue::CurrentSelectFrame);
				VideoObject >> Frame;
				cv::imwrite(("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "POS" + QString::number(GlobalValue::CurrentSelectFrame) + ".jpg").toLocal8Bit().toStdString(), Frame);
				cv::imwrite(("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cover.png").toLocal8Bit().toStdString(), Frame);
				VideoObject.release();
				EditWidget->setStyleSheet("QLabel#EditWidget{border-image:url('./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "POS" + QString::number(GlobalValue::CurrentSelectFrame) + ".jpg" + "')}");
			}
		}
		catch (cv::Exception& e) {
			qDebug() << e.what();

		}
	}
	void loadVideo(QString VideoDir) {
		PlayList->clear();
		PlayList->addMedia(QUrl::fromLocalFile(VideoDir));
		PlayList->setPlaybackMode(QMediaPlaylist::Loop); 
		PlayList->setCurrentIndex(0);
		GlobalValue::CurrentDuration = -1;
		//MediaPlayer->play();
	}
	void resizeEvent(QResizeEvent* event = Q_NULLPTR) {
		PlayControlButton->setGeometry(QRect(width() * 0.48, height()*0.99 - width() * 0.04, width() * 0.04, width() * 0.04));
		NextFrameButton->setGeometry(QRect(width() * 0.545, height()*0.99 - width() * 0.04, width() * 0.04, width() * 0.04));
		BackFrameButton->setGeometry(QRect(width() * 0.415, height()*0.99 - width() * 0.04, width() * 0.04, width() * 0.04));
		VideoWidget->setGeometry(QRect((width() - height() * 0.8 / 9 * 16) / 2, height() * 0.1, height() * 0.8 / 9 * 16, height() * 0.8));
		EditWidget->setGeometry(QRect((width() - height() * 0.8 / 9 * 16) / 2, height() * 0.1, height() * 0.8 / 9 * 16, height() * 0.8));
		PlayModeButton->setGeometry(QRect(width() * 0.27, height() * 0.01, width() * 0.22, height() * 0.08));
		EditModeButton->setGeometry(QRect(width() * 0.51, height() * 0.01, width() * 0.22, height() * 0.08));
		ToolFrameButton->setGeometry(QRect(width() * 0.90, height() * 0.2, width() * 0.06, width() * 0.06));
		ToolRootButton->setGeometry(QRect(width() * 0.90, height() * 0.35, width() * 0.06, width() * 0.06));
		CalculateButton->setGeometry(QRect(width() * 0.46, height() * 0.99 - width() * 0.04, width() * 0.08, width() * 0.04));
		CalculateButton->setStyleSheet("\
				QPushButton\
					{border:2px solid #1473E6;border-radius:10px;background-color:#555555;font-size:" + QString::number((int)(height() * 0.04)) + "px;color:#FFFFFF;font-family:'Microsoft YaHei';}\
				QPushButton:hover\
					{border:2px solid #1473E6;border-radius:10px;background-color:rgba(0,0,0,0);font-size:" + QString::number((int)(height() * 0.04)) + "px;color:#FFFFFF;font-family:'Microsoft YaHei';}\
				QPushButton:pressed\
					{border:2px solid #1473E6;border-radius:10px;background-color:#333333;font-size:" + QString::number((int)(height() * 0.04)) + "px;color:#FFFFFF;font-family:'Microsoft YaHei';}\
				");
	}
	void mousePressEvent(QMouseEvent* event) {
		if (CurrentState == EditState::EditMode) {
			QRect MaxLabelArea = EditWidget->geometry();
			if (MaxLabelArea.x() < event->pos().x() &&
				event->pos().x() < (MaxLabelArea.x() + MaxLabelArea.width()) &&
				MaxLabelArea.y() < event->pos().y() &&
				event->pos().y() < (MaxLabelArea.y() + MaxLabelArea.height())) {
				switch (ToolsState) {
				case ToolState::Rectangle:
					RectangleLabel->hide();
					RectangleLabel->setGeometry(QRect(0, 0, 0, 0));
					RectangleLabel->repaint();
					RectangleLabel->show();
					RectangleLabel->repaint();
					RectangleLabel->move(event->pos());
					break;
				case ToolState::Round:
					RoundLabel->hide();
					RoundLabel->repaint();
					RoundLabel->show();
					RoundLabel->repaint();
					RoundLabel->move(event->pos().x() - 10, event->pos().y() - 10);
					break;
				}
				
			}
		}
	}
	void mouseMoveEvent(QMouseEvent* event) {
		if (CurrentState == EditState::EditMode) {
			QRect MaxLabelArea = EditWidget->geometry();
			if (MaxLabelArea.x() < event->pos().x() &&
				event->pos().x() < (MaxLabelArea.x() + MaxLabelArea.width()) &&
				MaxLabelArea.y() < event->pos().y() &&
				event->pos().y() < (MaxLabelArea.y() + MaxLabelArea.height())) {
				switch (ToolsState) {
				case ToolState::Rectangle:
					RectangleLabel->resize(event->pos().x()-RectangleLabel->x(),event->pos().y()-RectangleLabel->y());
					break;
				}
			}
		}
	}
	void mouseReleaseEvent(QMouseEvent* event = Q_NULLPTR) {
		GlobalValue::RectangleGeo[0] = RectangleLabel->x() / (float)width();
		GlobalValue::RectangleGeo[1] = RectangleLabel->y() / (float)height();
		GlobalValue::RectangleGeo[2] = RectangleLabel->width() / (float)width();
		GlobalValue::RectangleGeo[3] = RectangleLabel->height() / (float)height();
		GlobalValue::RoundGeo[0] = RoundLabel->x() / (float)width();
		GlobalValue::RoundGeo[1] = RoundLabel->y() / (float)height();
		GlobalValue::RecGeo[0] = (float)(RectangleLabel->x() - VideoWidget->x()) / VideoWidget->width();
		GlobalValue::RecGeo[1] = (float)(RectangleLabel->y() - VideoWidget->y()) / VideoWidget->height();
		GlobalValue::RecGeo[2] = (float)(RectangleLabel->width()) / VideoWidget->width();
		GlobalValue::RecGeo[3] = (float)(RectangleLabel->height()) / VideoWidget->height();
		GlobalValue::RdGeo[0] = (float)(RoundLabel->x() + 10 - VideoWidget->x()) / VideoWidget->width();
		GlobalValue::RdGeo[1] = (float)(RoundLabel->y() + 10 - VideoWidget->y()) / VideoWidget->height();
		emit geoInfoChanged();
	}
};

class uSourceWidget :public QFrame
{
	Q_OBJECT
public:
	CE::CEButtonGroup* ButtonGroup;
	QScrollBar* ScrollBar;
	QWidget* CentralWidget;
	
	int Line = 0;
	uSourceWidget(QWidget*parent = Q_NULLPTR) {
		this->setParent(parent);

		ButtonGroup = new CE::CEButtonGroup(this);
		CentralWidget = new QWidget(this);
		ScrollBar = new QScrollBar(Qt::Vertical, this);
		connect(ScrollBar, SIGNAL(valueChanged(int)), this, SLOT(moveBigPad(int)));

		ScrollBar->setMinimum(0);
		ScrollBar->setMaximum(Line * 10);
		ScrollBar->setSingleStep(1);
		ScrollBar->setValue(0);
	}
	void refreshVideoList(QStringList VideoList) {
		int VideoNum = VideoList.length();
		if (!ButtonGroup->isEmpty()) {
			for (int i = 0; i < ButtonGroup->length(); i++) {
				ButtonGroup->button(i)->deleteLater();
			}
			ButtonGroup->clear();
		}
		for (int i = 0; i < VideoNum; i++) {
			CE::CEMultiButton* Button = new CE::CEMultiButton(CE::TILayout::VIT, CentralWidget, true);
			cv::VideoCapture VideoObject;
			cv::Mat Frame;
			//qDebug() << "./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + VideoList[i].section("/", -1, -1) + ".png";
			//qDebug() << VideoList[i];
			VideoObject.open(VideoList[i].toStdString());
			VideoObject.set(cv::CAP_PROP_POS_FRAMES, 10);
			VideoObject >> Frame;
			cv::imwrite(("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + VideoList[i].section("/", -1, -1) + ".jpg").toLocal8Bit().toStdString(), Frame);
			//cv::imwrite(("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cover.png").toLocal8Bit().toStdString(), Frame);
			VideoObject.release();
			Button->setCentralImage("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + VideoList[i].section("/", -1, -1) + ".jpg");
			Button->setText(VideoList[i].section("/", -1, -1));
			Button->setTextStyleSheet("QLabel{font-size:__AUTO__;color:#FFFFFF;font-family:'Microsoft YaHei'}");
			Button->InsiderValue = VideoList[i];
			Button->show();
			ButtonGroup->addCEMultiButton(Button);

		}
		resizeEvent();
		CentralWidget->setGeometry(QRect(0, height() * 0.02 , width() * 0.95, height() * 1.0 * 0.25 * (Line + 1)));
	}
	void resizeEvent(QResizeEvent* event = Q_NULLPTR) {
		this->setStyleSheet("QFrame{background-color:#333333};");
		
		if (!ButtonGroup->isEmpty()) {
			int Cow = 0;
			Line = 0;
			for (int i = 0; i < ButtonGroup->length(); i++) {
				ButtonGroup->button(i)->setGeometry(QRect(width() * 0.9 * 0.5 * Cow, height() * 1.0 * 0.25 * Line, width() * 0.9 * 0.5, height() * 1.0 * 0.25));
				Cow++;
				if (Cow == 2) {
					Cow = 0;
					Line++;
				}
			}
		}
		CentralWidget->setGeometry(QRect(0, CentralWidget->pos().y(), width() * 0.95, height() * 1.0 * 0.25 * (Line + 1)));
		ScrollBar->setGeometry(QRect(width() * 0.95, height() * 0, width() * 0.05, height() * 1));
		ScrollBar->setMinimum(0);
		ScrollBar->setMaximum(Line*10);
		ScrollBar->setSingleStep(1);
		ScrollBar->setStyleSheet("\
				QScrollBar:vertical{\
                    background-color:#2C2C2C;\
                    margin:0px,0px,0px,0px;\
                    padding-top:0px;\
                    padding-bottom:0px;\
                }\
                QScrollBar::handle:vertical{\
                    background-color:rgba(255,255,255,1);\
                    border-radius:" + QString::number((int)(width() * 0.025)) + "px;\
                }\
                QScrollBar::handle:vertical:hover{\
                    background-color:rgba(200,200,200,1);\
                    border-radius:" + QString::number((int)(width() * 0.025)) + "px;\
                }\
                QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{\
                    background-color:rgba(0,0,0,0);\
                }\
                QScrollBar::add-line:vertical{\
                    height:0px;\
                    width:0px;\
                    subcontrol-position:bottom;\
                }\
                QScrollBar::sub-line:vertical{\
                    height:0px;\
                    width:0px;\
                    subcontrol-position:top;\
                    }");
	}
public slots:
	void moveBigPad(int Value) {
		CentralWidget->setGeometry(QRect(0, height() * 0.02 - Value * height() * 0.1 * 0.25, width() * 0.95, height() * 1.0 * 0.25 * (Line + 1)));
	}
};

class uInfoWidget :public QFrame
{
	Q_OBJECT
public:
	QTextBrowser* KernalInfoBrowser;
	QString InsiderString;
	uInfoWidget(QWidget*parent = Q_NULLPTR) {
		this->setParent(parent);
		KernalInfoBrowser = new QTextBrowser(this);
		addInfo(GlobalValue::CurrentProject + "已准备就绪");
	}
	void resizeEvent(QResizeEvent* event) {
		KernalInfoBrowser->setStyleSheet("\
				QTextBrowser{\
					color:#FFFFFF;\
					font-family:'Microsoft YaHei';\
					background-color:#222222;\
					border:3px solid #222222;\
				}");
		KernalInfoBrowser->verticalScrollBar()->setStyleSheet("\
				QScrollBar:vertical{\
                    background-color:#2C2C2C;\
                    margin:0px,0px,0px,0px;\
                    padding-top:0px;\
                    padding-bottom:0px;\
                }\
                QScrollBar::handle:vertical{\
                    background-color:rgba(255,255,255,1);\
                    border-radius:" + QString::number((int)(width() * 0.06)) + "px;\
                }\
                QScrollBar::handle:vertical:hover{\
                    background-color:rgba(200,200,200,1);\
                    border-radius:" + QString::number((int)(width() * 0.06)) + "px;\
                }\
                QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{\
                    background-color:rgba(0,0,0,0);\
                }\
                QScrollBar::add-line:vertical{\
                    height:0px;\
                    width:0px;\
                    subcontrol-position:bottom;\
                }\
                QScrollBar::sub-line:vertical{\
                    height:0px;\
                    width:0px;\
                    subcontrol-position:top;\
                    }");
		KernalInfoBrowser->horizontalScrollBar()->setStyleSheet("\
				QScrollBar:horizontal{\
                    background-color:#2C2C2C;\
                    margin:0px,0px,0px,0px;\
                    padding-top:0px;\
                    padding-bottom:0px;\
                }\
                QScrollBar::handle:horizontal\
                    background-color:rgba(255,255,255,1);\
                    border-radius:" + QString::number((int)(width() * 0.06)) + "px;\
                }\
                QScrollBar::handle:horizontal:hover{\
                    background-color:rgba(200,200,200,1);\
                    border-radius:" + QString::number((int)(width() * 0.06)) + "px;\
                }\
                QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{\
                    background-color:rgba(0,0,0,0);\
                }\
                QScrollBar::add-line:horizontal{\
                    height:0px;\
                    width:0px;\
                    subcontrol-position:bottom;\
                }\
                QScrollBar::sub-line:horizontal{\
                    height:0px;\
                    width:0px;\
                    subcontrol-position:top;\
                    }");
		//this->setStyleSheet("QFrame{border-radius:2px solid #AAAAAA;}");
		KernalInfoBrowser->setGeometry(QRect(0, 0, width(), height()));
	}
public slots:
	void addInfo(QString input) {
		InsiderString += (input + "\n");
		KernalInfoBrowser->setText(InsiderString);
		KernalInfoBrowser->textCursor().movePosition(QTextCursor::End);
	}
};

class MainDef :public QWidget
{
	Q_OBJECT
public:
	uProgressWidget* ProgressArea;
	uINPlayerWidget* PlayerArea;
	uSourceWidget* SourceArea;
	uInfoWidget* InfoArea;
	QLabel* SourceTitle;
	QPushButton* ManageButton;
	SelectVideos* ManagePage;
	
	//QPushButton* TestButton;
	CE::CESettings* Settings;
	MainDef(QWidget* parent = Q_NULLPTR) {
		ProgressArea = new uProgressWidget(this);
		PlayerArea = new uINPlayerWidget(this);
		SourceArea = new uSourceWidget(this);
		SourceTitle = new QLabel(this);
		SourceTitle->setText("  项目资源查看器");
		//TestButton = new QPushButton(this);
		Settings = new CE::CESettings("./Users_Data/repos/" + GlobalValue::CurrentProject + "/Project.ces", this);
		InfoArea = new uInfoWidget(this);
		ManageButton = new QPushButton(this);
		ManageButton->setText("更改");
		
		//connect(TestButton, SIGNAL(clicked()), this, SLOT(play()));
		connect(SourceArea->ButtonGroup, SIGNAL(refreshed()), this, SLOT(changeVideo()));
		connect(ProgressArea, SIGNAL(countChanged(int)), this, SLOT(spawnVideoInfo(int)));
		connect(PlayerArea->MediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(changeDuration(qint64)));
		connect(PlayerArea->MediaPlayer, SIGNAL(positionChanged(qint64)), ProgressArea, SLOT(distributeValue(qint64)));
		connect(ProgressArea, SIGNAL(valueChanged(qint64)), this, SLOT(changeValue(qint64)));
		connect(ManageButton, SIGNAL(clicked()), this, SLOT(openManagePage()));
		connect(PlayerArea->NextFrameButton, SIGNAL(clicked()), this, SLOT(frameNext()));
		connect(PlayerArea->BackFrameButton, SIGNAL(clicked()), this, SLOT(frameBack()));
		connect(PlayerArea, SIGNAL(geoInfoChanged()), this, SLOT(changeGeometryInfo()));
		
		initSettings();
		readMediaList();
	}
	void initSettings() {
		QMap<QString, QString> SettingsList;
		SettingsList["ProjectEdition"] = "";
		SettingsList["ProjectSource"] = "";
		SettingsList["ProjectStep"] = "";
		SettingsList["ProjectCache"] = "";
		SettingsList["ProjectThread"] = "";
		SettingsList["VideoGeometry"] = "";
		Settings->setKVPair(SettingsList);
		Settings->loadSettings();
		setMaxAliveThread();
	}
	void setMaxAliveThread() {
		GlobalValue::MaxAliveThread = Settings->valueOf("ProjectThread").toInt();
		if (GlobalValue::MaxAliveThread < 0 || GlobalValue::MaxAliveThread>256) {
			Settings->setValue("ProjectThread", "4");
			GlobalValue::MaxAliveThread = 4;
		}
		qDebug() << GlobalValue::MaxAliveThread;
	}
	void readMediaList() {
		QString SourceRAW = Settings->valueOf("ProjectSource").replace("\\", "/").replace("__PROJECT_LOCAL_RESOURCE__", QDir::currentPath().replace("\\","/") + "/Users_Data/repos/" + GlobalValue::CurrentProject + "/resource");
		qDebug() << SourceRAW;
		QStringList Source = SourceRAW.split("||");
		SourceArea->refreshVideoList(Source);
	}
	void resizeEvent(QResizeEvent* event) {
		SourceArea->setGeometry(QRect(width() * 0, height() * 0.05, width() * 0.3 + 1, height() * 0.65 + 1));
		SourceTitle->setGeometry(QRect(width() * 0, height() * 0, width() * 0.3 + 1, height() * 0.05 + 1));
		SourceTitle->setStyleSheet("QLabel{background-color:#393939;font-size:" + QString::number((int)(height() * 0.02)) + "px;color:#FFFFFF;font-family:'Microsoft YaHei';}");
		ProgressArea->setGeometry(QRect(width() * 0.3, height() * 0.7, width() * 0.7 + 1, height() * 0.3 + 1));
		PlayerArea->setGeometry(QRect(width() * 0.3, height() * 0, width() * 0.7 + 1, height() * 0.7 + 1));
		InfoArea->setGeometry(QRect(width() * 0, height() * 0.7, width() * 0.3 + 1, height() * 0.3 + 1));
		ManageButton->setGeometry(QRect(width() * 0.25, height() * 0.008, width() * 0.04, height() * 0.035));
		ManageButton->setStyleSheet("\
				QPushButton\
					{border:2px solid #1473E6;border-radius:10px;background-color:#555555;font-size:" + QString::number((int)(height() * 0.015)) + "px;color:#FFFFFF;font-family:'Microsoft YaHei';}\
				QPushButton:hover\
					{border:2px solid #1473E6;border-radius:10px;background-color:rgba(0,0,0,0);font-size:" + QString::number((int)(height() * 0.015)) + "px;color:#FFFFFF;font-family:'Microsoft YaHei';}\
				QPushButton:pressed\
					{border:2px solid #1473E6;border-radius:10px;background-color:#333333;font-size:" + QString::number((int)(height() * 0.015)) + "px;color:#FFFFFF;font-family:'Microsoft YaHei';}\
				");
		
		for (int i = 0; i < ProgressArea->RawVideo->count(); i++) {
			ProgressArea->setRawImage(i, "./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "PRO" + QString::number((int)(29 / ProgressArea->RawVideo->count()) * (i + 1)) + ".jpg");
		}
	}
public slots:
	
	void frameBack() {
		ProgressArea->distributeValueWithSignal(ProgressArea->value() - GlobalValue::CurrentDuration / GlobalValue::CurrentFrame);
	}
	void frameNext() {
		ProgressArea->distributeValueWithSignal(ProgressArea->value() + GlobalValue::CurrentDuration / GlobalValue::CurrentFrame);
	}
	void openManagePage() {
		ManagePage = new SelectVideos(Settings->valueOf("ProjectSource"));
		connect(ManagePage, SIGNAL(currentList(QString)), this, SLOT(changeProjectSource(QString)));
		connect(ManagePage, SIGNAL(closed()), this, SLOT(closeManagePage()));
		//this->parentWidget()->hide();
		ManagePage->setWindowModality(Qt::ApplicationModal);
		ManagePage->show();
	}
	void closeManagePage() {
		//this->parentWidget()->show();
		ManagePage->disconnect();
		ManagePage->deleteLater();
	}
	void changeProjectSource(QString NewSource) {
		Settings->setValue("ProjectSource", NewSource);
		readMediaList();
	}
	void changeValue(qint64 Value) {
		PlayerArea->setPosition(Value);
	}
	void changeDuration(qint64 Duration) {
		ProgressArea->setMaximum(Duration);
		GlobalValue::CurrentDuration = Duration;
		ProgressArea->setMinimum(0);
		if (GlobalValue::loadEditFrame != -1) {
			ProgressArea->distributeValueWithSignal((float)GlobalValue::loadEditFrame / GlobalValue::CurrentFrame * GlobalValue::CurrentDuration);
			GlobalValue::loadEditFrame = -1;
		}
	}
	void spawnVideoInfo(int JustASlot) {
		if (GlobalValue::CurrentDuration == -1) {
			cv::VideoCapture VideoObject;
			cv::Mat Frame;
			qDebug() << GlobalValue::CurrentVideo;
			
			VideoObject.open(GlobalValue::CurrentVideo.toStdString());
			GlobalValue::CurrentFrame = VideoObject.get(cv::CAP_PROP_FRAME_COUNT);
			qDebug() << GlobalValue::CurrentFrame;
			for (int i = 0; i < 29; i++) {
				QFile cacheFile = "./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\","/").section("/", -1, -1) + "PRO" + QString::number(i + 1) + ".jpg";
				if (!cacheFile.exists()) {
					qDebug() << (int)(GlobalValue::CurrentFrame / 30) * (i + 1);
					qDebug() << "./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "PRO" + QString::number(i + 1) + ".jpg";
					VideoObject.set(cv::CAP_PROP_POS_FRAMES, (int)((float)GlobalValue::CurrentFrame / 30) * (i + 1));
					VideoObject >> Frame;
					//imshow("", Frame);
					cv::imwrite(("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "PRO" + QString::number(i + 1) + ".jpg").toLocal8Bit().toStdString(), Frame);
				}
			}
			for (int i = 0; i < ProgressArea->RawVideo->count(); i++) {
				ProgressArea->setRawImage(i, "./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "PRO" + QString::number((int)(29 / ProgressArea->RawVideo->count()) * (i + 1)) + ".jpg");
			}
			VideoObject.release();
			
		}
		
	}
	bool changeVideo() {
		GlobalValue::CurrentVideo = SourceArea->ButtonGroup->currentValue();
		
		PlayerArea->RectangleLabel->hide();
		PlayerArea->RoundLabel->hide();
		PlayerArea->RectangleLabel->repaint();
		PlayerArea->RoundLabel->repaint();
		PlayerArea->RectangleLabel->setGeometry(QRect(0, 0, 0, 0));
		PlayerArea->RoundLabel->move(0, 0);
		qDebug() << GlobalValue::CurrentVideo;
		PlayerArea->loadVideo(GlobalValue::CurrentVideo);
		GlobalValue::loadEditFrame = -1;
		spawnVideoInfo(0);
		QString GeoInfo = Settings->valueOf("VideoGeometry");
		QStringList GeoList = GeoInfo.split("||||");
		if (!GeoList.isEmpty()) {
			for (int i = 0; i < GeoList.length(); i++) {
				if (GeoList[i].contains(GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1))) {
					QStringList Geo = GeoList[i].split("||");
					PlayerArea->loadGeometryInfo(Geo[1].toFloat(), Geo[2].toFloat(), Geo[3].toFloat(), Geo[4].toFloat(), Geo[5].toFloat(), Geo[6].toFloat());
					GlobalValue::loadEditFrame = Geo[7].toInt();
					PlayerArea->EditModeButton->click();
					return TRUE;
				}
			}
		}
		if (GlobalValue::loadEditFrame == -1) {
			if (PlayerArea->CurrentState == uINPlayerWidget::EditState::PlayMode) {
				PlayerArea->PlayControlButton->click();
			}
			else {
				PlayerArea->MediaPlayer->setPosition(0);
				emit PlayerArea->MediaPlayer->positionChanged(0);
				PlayerArea->MediaPlayer->pause();
			}
		}
	}
	bool changeGeometryInfo() {
		if (GlobalValue::RectangleGeo[0] != 0 && GlobalValue::RectangleGeo[1] != 0 && GlobalValue::RoundGeo[0] != 0 && GlobalValue::RoundGeo[1] != 0) {
			QString GeoInfo = Settings->valueOf("VideoGeometry");
			QStringList GeoList = GeoInfo.split("||||");
			QString CurrentInfo = GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "||"
				+ QString::number(GlobalValue::RectangleGeo[0]) + "||" + QString::number(GlobalValue::RectangleGeo[1]) + "||"
				+ QString::number(GlobalValue::RectangleGeo[2]) + "||" + QString::number(GlobalValue::RectangleGeo[3]) + "||"
				+ QString::number(GlobalValue::RoundGeo[0]) + "||" + QString::number(GlobalValue::RoundGeo[1]) + "||"
				+ QString::number(GlobalValue::CurrentSelectFrame);
			if (!GeoList.isEmpty()) {
				for (int i = 0; i < GeoList.length(); i++) {
					if (GeoList[i].contains(GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1))) {
						GeoList[i] = CurrentInfo;
						Settings->setValue("VideoGeometry", GeoList.join("||||"));
						return TRUE;
					}
				}
			}
			Settings->setValue("VideoGeometry", GeoInfo + "||||" + CurrentInfo);
		}
	}
	void backToManager() {
		saveProject();
		qApp->exit(0);
	}
	void saveProject() {
		PlayerArea->mouseReleaseEvent();
		Settings->save();
		QDir ChangeTime;
		ChangeTime.setPath("./Users_Data/repos/" + GlobalValue::CurrentProject + "");
		ChangeTime.mkdir("TIME");
		ChangeTime.setPath("./Users_Data/repos/" + GlobalValue::CurrentProject + "/TIME");
		ChangeTime.removeRecursively();
	}
	void saveAndQuit() {
		saveProject();
		qApp->exit(233);
	}
};

class MainWindow :public QMainWindow
{
	Q_OBJECT
public:
	MainDef* CentralWidget;
	QMenuBar* MenuBar;
	QMenu* File;
	QAction* OpenFile, * SaveFile, * CloseProgram;
	QMenu* Project;
	QAction* ClearCache, * ClearOutput, * ClearAll,* ChangeThread, * Local;
	QMenu* Help;
	QAction* About, * Document;
	MainWindow(QWidget* parent = Q_NULLPTR) {
		if (Program_Settings("Window_SizePolicy") == "F") {
			setWindowState(Qt::WindowMaximized);
		}
		else {
			this->resize(Program_Settings("Window_Size").split("_")[0].toInt(), Program_Settings("Window_Size").split("_")[1].toInt());
		}
		this->setMinimumSize(1366, 768);
		this->setWindowTitle("Firework");
		CentralWidget = new MainDef(this);
		this->setCentralWidget(CentralWidget);
		initMenu();
	}
	void initMenu() {
		MenuBar = new QMenuBar(this);
		File = new QMenu("文件", this);
		OpenFile = new QAction("打开/新建", this);
		SaveFile = new QAction("保存项目", this);
		CloseProgram = new QAction("关闭程序", this);
		File->addAction(OpenFile);
		File->addAction(SaveFile);
		File->addSeparator();
		File->addAction(CloseProgram);

		Project = new QMenu("项目", this);
		ClearCache = new QAction("清理缓存并关闭项目", this);
		ClearOutput = new QAction("清理生成结果", this);
		ClearAll = new QAction("清理全部内容并关闭项目", this);
		ChangeThread = new QAction("更改多线程选项", this);
		Local = new QAction("项目本地化", this);

		Project->addAction(ClearCache);
		Project->addAction(ClearOutput);
		Project->addAction(ClearAll);
		Project->addSeparator();
		Project->addAction(ChangeThread);
		Project->addSeparator();
		Project->addAction(Local);

		Help = new QMenu("帮助", this);
		About = new QAction("关于", this);
		Document = new QAction("帮助文档", this);
		Help->addAction(About);
		Help->addSeparator();
		Help->addAction(Document);

		MenuBar->addMenu(File);
		MenuBar->addMenu(Project);
		MenuBar->addMenu(Help);

		this->setMenuBar(MenuBar);
		connect(OpenFile, SIGNAL(triggered()), CentralWidget, SLOT(backToManager()));
		connect(SaveFile, SIGNAL(triggered()), CentralWidget, SLOT(saveProject()));
		connect(CloseProgram, SIGNAL(triggered()), CentralWidget, SLOT(saveAndQuit()));
		connect(About, SIGNAL(triggered()), this, SLOT(openAboutPage()));
		connect(ClearOutput, SIGNAL(triggered()), this, SLOT(clearOutputFolder()));
		connect(ClearCache, SIGNAL(triggered()), this, SLOT(clearCacheFolder()));
		connect(ClearAll, SIGNAL(triggered()), this, SLOT(clearAllFolder()));
		connect(ChangeThread, SIGNAL(triggered()), this, SLOT(changeThread()));
		connect(Local, SIGNAL(triggered()), this, SLOT(localProject()));
		connect(Document, SIGNAL(triggered()), this, SLOT(openHelp()));
		this->setStyleSheet("\
			QWidget{font-size:17px}\
			QMenuBar{background-color:#272727;color:#FFFFFF;font-family:'Microsoft YaHei'}\
			QMenuBar::item::selected{background-color:#333333;color:#FFFFFF;font-family:'Microsoft YaHei'}\
			QMenu,QMenu::item{background-color:#272727;color:#FFFFFF;font-family:'Microsoft YaHei'}\
			QMenu::item::selected{background-color:#444444;color:#FFFFFF;font-family:'Microsoft YaHei'}");
	}
public slots:
	void openHelp() {
		system((QDir::currentPath()+"/Users_Data/dev/help.pdf").toLocal8Bit());
	}
	void changeThread() {
		ThreadSettingsPage* newPage = new ThreadSettingsPage();
		connect(newPage, SIGNAL(isClosed()), this, SLOT(saveThreadChange()));
		newPage->show();
	}
	void saveThreadChange() {
		CentralWidget->Settings->setValue("ProjectThread", QString::number(GlobalValue::MaxAliveThread));
	}
	void clearAllFolder() {
		clearOutputFolder();
		clearCacheFolder();
	}
	void clearOutputFolder() {
		CentralWidget->InfoArea->addInfo("正在清除输出文件夹");
		CentralWidget->InfoArea->repaint();
		CentralWidget->InfoArea->addInfo("清理期间，程序会停止响应");
		CentralWidget->InfoArea->repaint();
		QDir output;
		output.setPath("./Users_Data/repos/" + GlobalValue::CurrentProject + "/output");
		output.removeRecursively();
		output.setPath("./Users_Data/repos/" + GlobalValue::CurrentProject );
		output.mkdir("output");
		CentralWidget->InfoArea->addInfo("已经清理当前项目的输出文件夹");
	}
	void clearCacheFolder() {
		CentralWidget->InfoArea->addInfo("正在清除缓存文件夹");
		CentralWidget->InfoArea->repaint();
		CentralWidget->InfoArea->addInfo("清理期间，程序会停止响应");
		CentralWidget->InfoArea->repaint();
		QDir output;
		output.setPath("./Users_Data/repos/" + GlobalValue::CurrentProject + "/frame");
		output.removeRecursively();
		output.setPath("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache");
		output.removeRecursively();
		output.setPath("./Users_Data/repos/" + GlobalValue::CurrentProject);
		output.mkdir("frame");
		output.mkdir("cache");
		CentralWidget->InfoArea->addInfo("已经清理当前项目的缓存文件夹");
		CentralWidget->InfoArea->repaint();
		Sleep(1000);
		CentralWidget->backToManager();
	}
	void localProject() {
		CentralWidget->InfoArea->addInfo("正在执行项目本地化");
		CentralWidget->InfoArea->addInfo("执行本地化期间，程序会停止响应");
		CentralWidget->InfoArea->addInfo("正在遍历链入资源");
		QString Source = CentralWidget->Settings->valueOf("ProjectSource");
		QStringList SourceList = Source.split("||");
		QFile CopySourceFile;
		CentralWidget->InfoArea->addInfo("总共链入资源" + QString::number(SourceList.length()) + "个");
		for (int i = 0; i < SourceList.length(); i++) {
			CentralWidget->InfoArea->addInfo("正在复制第" + QString::number(i + 1) + "个");
			CentralWidget->InfoArea->repaint();
			QFile::copy(SourceList[i], "./Users_Data/repos/" + GlobalValue::CurrentProject + "/resource/" + SourceList[i].replace("\\", "/").section("/", -1, -1));
			SourceList[i] = "__PROJECT_LOCAL_RESOURCE__/" + SourceList[i].replace("\\", "/").section("/", -1, -1);
		}
		CentralWidget->Settings->setValue("ProjectSource", SourceList.join("||"));
		CentralWidget->InfoArea->addInfo("项目本地化已经完成");
		CentralWidget->InfoArea->repaint();
	}
	void openAboutPage() {
		CE::CEInfoPage* InfoPage = new CE::CEInfoPage();
		InfoPage->setWindowTitle("关于");
		InfoPage->setText("\
Firework\n\
南京工业大学 过程安全实验室\n\
\n\
本软件由 何俊义、王智 开发\n\
本软件使用了由 何俊义 开发的另一项目CommonEdit用于实现部分页面\n\
软件版本V1.0");
		InfoPage->setStyleSheet("\
			QFrame{\
				background-color:#333333;\
				color:#FFFFFF;\
				font-family:'Microsoft YaHei';\
			}QPushButton{\
				border:2px solid #555555;\
				border-radius:5px;\
				color:#FFFFFF;\
			}QPushButton:hover{\
				border:2px solid #555555;\
				border-radius:5px;\
				color:#FFFFFF;\
				background-color:#444444;\
			}QPushButton:pressed{\
				border:2px solid #555555;\
				border-radius:5px;\
				color:#FFFFFF;\
				background-color:#333333;\
			}");
		InfoPage->show();

	}
public slots:
	void resizeEvent(QResizeEvent* event = Q_NULLPTR) {
		if (width() >= 1366 || height() >= 768) {
			writesettings("Window_Size", QString::number(width()) + "_" + QString::number(height()));
		}
		MenuBar->setFixedWidth(width());
	}
	void changeEvent(QEvent* event = Q_NULLPTR) {
		if (event->type() == QEvent::WindowStateChange) {
			if (windowState() == Qt::WindowMaximized) {
				writesettings("Window_SizePolicy", "F");
			}
			else {
				writesettings("Window_SizePolicy", "N");
			}
		}
	}
	void closeEvent(QCloseEvent* event = Q_NULLPTR) {
		CentralWidget->saveAndQuit();
	}
};
