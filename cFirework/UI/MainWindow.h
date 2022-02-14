#pragma once
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtMultimedia>
#include "Widget.h"
#include "SelectVideos.h"
#include "../Func/FrameDelay.h"
#include "../Func/keyFunction.h"
#include "CEWidgets.h"
#include "../Func/ProjectCES.h"
#include "../global_value.h"
#include "../loadsettings.h"
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
	void setRawImage(int value,QString ImageDir) {
		RawVideo->loadImage(value, ImageDir);
	}

};

class uINPlayerWidget :public QFrame
{
	Q_OBJECT
public:
	enum class EditState
	{
		Unknown = 0,
		PlayMode = 1,
		EditMode = 2,
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
	EditState CurrentState = EditState::PlayMode;
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
			QPushButton{border-image:url(':IRC/Icon/StopButton');background-color:#222222;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:hover{border-image:url(':IRC/Icon/StopButton');background-color:#555555;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:pressed{border-image:url(':IRC/Icon/StopButton');background-color:#888888;border:2px solid #FFFFFF;border-radius:5px}");
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
	}
public slots:
	void changeState() {
		QString SelectWhich =ButtonGroup->currentString();
		if (SelectWhich == "回放模式") {
			CurrentState = EditState::PlayMode;
			PlayControlButton->show();
			VideoWidget->raise();
		}
		else {
			CurrentState = EditState::EditMode;
			PlayControlButton->hide();
			EditWidget->raise();
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
	void playerStateChange() {
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
			QPushButton{border-image:url(':IRC/Icon/PlayButton');background-color:#222222;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:hover{border-image:url(':IRC/Icon/PlayButton');background-color:#555555;border:2px solid #FFFFFF;border-radius:5px}\
			QPushButton:pressed{border-image:url(':IRC/Icon/PlayButton');background-color:#888888;border:2px solid #FFFFFF;border-radius:5px}");
			break;
		}
	}
	void setPosition(qint64 Position) {
		switch (CurrentState) {
		case EditState::PlayMode:
			MediaPlayer->setPosition(Position);
			break;
		case EditState::EditMode:
			exportCurrentFrame(Position);
			break;
		}
	}
	void exportCurrentFrame(qint64 Position) {
		try {
			cv::VideoCapture VideoObject;
			cv::Mat Frame;
			VideoObject.open(GlobalValue::CurrentVideo.toStdString());
			qDebug() << VideoObject.get(cv::CAP_PROP_FRAME_COUNT);
			qDebug() << Position;
			VideoObject.set(cv::CAP_PROP_POS_FRAMES, (int)((double)Position / MediaPlayer->duration() * VideoObject.get(cv::CAP_PROP_FRAME_COUNT)));
			VideoObject >> Frame;
			cv::imwrite(("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "POS" + QString::number(Position) + ".jpg").toLocal8Bit().toStdString(), Frame);
			VideoObject.release();
			EditWidget->setStyleSheet("QLabel#EditWidget{border-image:url('./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "POS" + QString::number(Position) + ".jpg" + "')}");
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
		qDebug() << VideoWidget->frameSize();
		qDebug() << VideoWidget->size();
		//MediaPlayer->play();
	}
	void resizeEvent(QResizeEvent* event = Q_NULLPTR) {
		PlayControlButton->setGeometry(QRect(width() * 0.48, height()*0.99 - width() * 0.04, width() * 0.04, width() * 0.04));
		NextFrameButton->setGeometry(QRect(width() * 0.53, height()*0.99 - width() * 0.04, width() * 0.04, width() * 0.04));
		BackFrameButton->setGeometry(QRect(width() * 0.43, height()*0.99 - width() * 0.04, width() * 0.04, width() * 0.04));
		VideoWidget->setGeometry(QRect((width() - height() * 0.8 / 9 * 16) / 2, height() * 0.1, height() * 0.8 / 9 * 16, height() * 0.8));
		EditWidget->setGeometry(QRect((width() - height() * 0.8 / 9 * 16) / 2, height() * 0.1, height() * 0.8 / 9 * 16, height() * 0.8));
		PlayModeButton->setGeometry(QRect(width() * 0.27, height() * 0.01, width() * 0.22, height() * 0.08));
		EditModeButton->setGeometry(QRect(width() * 0.51, height() * 0.01, width() * 0.22, height() * 0.08));
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
			cv::imwrite(("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cover.png").toLocal8Bit().toStdString(), Frame);
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
	uInfoWidget(QWidget*parent = Q_NULLPTR) {
		this->setParent(parent);
	}
	void resizeEvent(QResizeEvent* event) {
		this->setStyleSheet("QFrame{background-color:#222222};");
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
		SourceTitle->setText("  项目资源管理器");
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
		Settings->setKVPair(SettingsList);
		Settings->loadSettings();
	}
	void readMediaList() {
		QString SourceRAW = Settings->valueOf("ProjectSource").replace("\\","/");
		QStringList Source = SourceRAW.split("||");
		SourceArea->refreshVideoList(Source);
	}
	void resizeEvent(QResizeEvent* event) {
		SourceArea->setGeometry(QRect(width() * 0, height() * 0.05, width() * 0.3 + 1, height() * 0.65 + 1));
		SourceTitle->setGeometry(QRect(width() * 0, height() * 0, width() * 0.3 + 1, height() * 0.05 + 1));
		SourceTitle->setStyleSheet("QLabel{background-color:#393939;font-size:" + QString::number((int)(height() * 0.02)) + "px;color:#FFFFFF;font-family:'Microsoft YaHei'}");
		ProgressArea->setGeometry(QRect(width() * 0.3, height() * 0.7, width() * 0.7 + 1, height() * 0.3 + 1));
		PlayerArea->setGeometry(QRect(width() * 0.3, height() * 0, width() * 0.7 + 1, height() * 0.7 + 1));
		InfoArea->setGeometry(QRect(width() * 0, height() * 0.7, width() * 0.3 + 1, height() * 0.3 + 1));
		ManageButton->setGeometry(QRect(width() * 0.25, height() * 0.008, width() * 0.04, height() * 0.035));
		ManageButton->setStyleSheet("\
				QPushButton\
					{border:2px solid #1473E6;border-radius:10px;background-color:#555555;font-size:" + QString::number((int)(height() * 0.015)) + "px;color:#FFFFFF;}\
				QPushButton:hover\
					{border:2px solid #1473E6;border-radius:10px;background-color:rgba(0,0,0,0);font-size:" + QString::number((int)(height() * 0.015)) + "px;color:#FFFFFF;}\
				QPushButton:pressed\
					{border:2px solid #1473E6;border-radius:10px;background-color:#333333;font-size:" + QString::number((int)(height() * 0.015)) + "px;color:#FFFFFF;}\
				");
		for (int i = 0; i < ProgressArea->RawVideo->count(); i++) {
			ProgressArea->setRawImage(i, "./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "PRO" + QString::number((int)(29 / ProgressArea->RawVideo->count()) * (i + 1)) + ".jpg");
		}
	}
public slots:
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
		ProgressArea->setMinimum(0);
	}
	void spawnVideoInfo(int JustASlot) {
		if (GlobalValue::CurrentDuration == -1) {
			cv::VideoCapture VideoObject;
			cv::Mat Frame;
			qDebug() << GlobalValue::CurrentVideo;
			
			VideoObject.open(GlobalValue::CurrentVideo.toStdString());
			GlobalValue::CurrentDuration = VideoObject.get(cv::CAP_PROP_FRAME_COUNT);
			qDebug() << GlobalValue::CurrentDuration;
			for (int i = 0; i < 29; i++) {
				QFile cacheFile = "./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\","/").section("/", -1, -1) + "PRO" + QString::number(i + 1) + ".jpg";
				if (!cacheFile.exists()) {
					qDebug() << (int)(GlobalValue::CurrentDuration / 30) * (i + 1);
					qDebug() << "./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "PRO" + QString::number(i + 1) + ".jpg";
					VideoObject.set(cv::CAP_PROP_POS_FRAMES, (int)(GlobalValue::CurrentDuration / 30) * (i + 1));
					VideoObject >> Frame;
					//imshow("", Frame);
					cv::imwrite(("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "PRO" + QString::number(i + 1) + ".jpg").toLocal8Bit().toStdString(), Frame);
				}
			}
			for (int i = 0; i < ProgressArea->RawVideo->count(); i++) {
				ProgressArea->setRawImage(i, "./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "PRO" + QString::number((int)(29 / ProgressArea->RawVideo->count()) * (i + 1)) + ".jpg");
			}
			VideoObject.release();
			if (PlayerArea->CurrentState == uINPlayerWidget::EditState::PlayMode) {
				PlayerArea->MediaPlayer->play();
			}
			else {
				PlayerArea->MediaPlayer->setPosition(0);
				emit PlayerArea->MediaPlayer->positionChanged(0);
				PlayerArea->MediaPlayer->pause();
			}
		}
		
	}
	void changeVideo() {
		GlobalValue::CurrentVideo = SourceArea->ButtonGroup->currentValue();
		qDebug() << GlobalValue::CurrentVideo;
		PlayerArea->loadVideo(GlobalValue::CurrentVideo);
		spawnVideoInfo(0);

	}
	void backToManager() {
		saveProject();
		qApp->exit(0);
	}
	void saveProject() {
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
		OpenFile = new QAction("回到项目管理器（打开或新建）", this);
		SaveFile = new QAction("保存项目", this);
		CloseProgram = new QAction("关闭程序", this);
		File->addAction(OpenFile);
		File->addAction(SaveFile);
		File->addSeparator();
		File->addAction(CloseProgram);

		MenuBar->addMenu(File);
		this->setMenuBar(MenuBar);
		connect(OpenFile, SIGNAL(triggered()), CentralWidget, SLOT(backToManager()));
		connect(SaveFile, SIGNAL(triggered()), CentralWidget, SLOT(saveProject()));
		connect(CloseProgram, SIGNAL(triggered()), CentralWidget, SLOT(saveAndQuit()));

		this->setStyleSheet("\
			QMenuBar{background-color:#272727;color:#FFFFFF;font-family:'Microsoft YaHei'}\
			QMenuBar::item::selected{background-color:#333333;color:#FFFFFF;font-family:'Microsoft YaHei'}\
			QMenu,QMenu::item{background-color:#272727;color:#FFFFFF;font-family:'Microsoft YaHei'}\
			QMenu::item::selected{background-color:#444444;color:#FFFFFF;font-family:'Microsoft YaHei'}");
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