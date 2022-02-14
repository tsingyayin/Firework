#pragma once
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include <windows.h>
#include "../loadsettings.h"
#include "../global_value.h"
#pragma execution_character_set("utf-8")
class uNewProjectPage :public QWidget
{
	Q_OBJECT
public:
	enum class InitProject {
		Unknown = 0,
		Empty = 1,
		Default = 2,
	};
	QLabel* TitleLabel;
	QLabel* ProjectName_Label;
	QLineEdit* ProjectName_Edit;
	QLabel* ProjectSource_Label;
	QLineEdit* ProjectSource_Edit;
	QPushButton* ProjectSource_Button;
	QLabel* ProjectCache_Label;
	QComboBox* ProjectCache_Box;
	QLabel* ProjectThread_Label;
	QComboBox* ProjectThread_Box;
	QLabel* NoticeLabel;
	SYSTEM_INFO SysInfo;
	QPushButton* StartButton;
	QPushButton* CancelButton;
	int Processors;
	uNewProjectPage(InitProject ProjectInit = InitProject::Default) {
		this->setWindowTitle("Firework 新建项目页");
		this->setMinimumSize(800, 600);
		TitleLabel = new QLabel(this);
		TitleLabel->setText("  新建");
		
		ProjectName_Label = new QLabel(this);
		ProjectName_Edit = new QLineEdit(this);
		ProjectSource_Label = new QLabel(this);
		ProjectSource_Edit = new QLineEdit(this);
		ProjectSource_Button = new QPushButton(this);
		ProjectCache_Label = new QLabel(this);
		ProjectCache_Box = new QComboBox(this);
		ProjectThread_Label = new QLabel(this);
		ProjectThread_Box = new QComboBox(this);

		NoticeLabel = new QLabel (this);
		StartButton = new QPushButton(this);
		CancelButton = new QPushButton(this);

		ProjectName_Label->setText("项目名称");
		ProjectSource_Label->setText("分析目标");
		ProjectSource_Button->setText("预览");
		ProjectCache_Label->setText("缓存管理");
		ProjectThread_Label->setText("最大线程");

		NoticeLabel->setAlignment(Qt::AlignCenter);
		StartButton->setText("开始");
		CancelButton->setText("取消");

		ProjectCache_Box->addItem("持续缓存", 0);
		ProjectCache_Box->addItem("临时缓存", 1);

		GetSystemInfo(&SysInfo);
		Processors = SysInfo.dwNumberOfProcessors;
		for (int i = 1; i <= Processors; i++) {
			ProjectThread_Box->addItem(QString::number(i), i - 1);
		}
		

		InitInfo(ProjectInit);
		connect(ProjectSource_Button, SIGNAL(clicked()), this, SLOT(chooseFile()));
		connect(StartButton, SIGNAL(clicked()), this, SLOT(createProject()));
		connect(CancelButton, SIGNAL(clicked()), this, SLOT(exitPage()));
	}
	void InitInfo(InitProject ProjectInit) {
		switch (ProjectInit) {
		case InitProject::Default:
			ProjectName_Edit->setText("未命名范例项目" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
			ProjectSource_Edit->setText(QDir::currentPath() + "/" + "Users_Data/dev/video/eg.mp4");
			ProjectThread_Box->setCurrentIndex(ProjectThread_Box->count() - 1);
			ProjectCache_Box->setCurrentIndex(0);
			break;
		case InitProject::Empty:
			ProjectName_Edit->setText("未命名项目" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
			ProjectSource_Edit->setText("");
			ProjectCache_Box->setCurrentIndex(-1);
			ProjectThread_Box->setCurrentIndex(-1);
			break;
		}
	}
	
	void resizeEvent(QResizeEvent* event = Q_NULLPTR) {
		this->setStyleSheet("QWidget{background-color:#444444;color:#FFFFFF;font-size:" + QString::number((int)(height() * 0.03)) + "px;font-family:'Microsoft YaHei'}");
		TitleLabel->setStyleSheet("QLabel{background-color:#333333;color:#FFFFFF;font-size:" + QString::number((int)(height() * 0.05)) + "px;font-family:'Microsoft YaHei'}");
		TitleLabel->setGeometry(QRect(width() * 0.0, height() * 0.0, width() * 1.0, height() * 0.1));
		ProjectName_Label->setGeometry(QRect(width() * 0.15, height() * 0.2, width() * 0.2, height() * 0.1));
		ProjectName_Edit->setGeometry(QRect(width() * 0.35, height() * 0.2, width() * 0.5, height() * 0.1));
		ProjectName_Edit->setStyleSheet("QLineEdit{background-color:#3C3C3C;border:1px solid #4A4A4A;border-radius:5px}");
		ProjectSource_Label->setGeometry(QRect(width() * 0.15, height() * 0.35, width() * 0.2, height() * 0.1));
		ProjectSource_Edit->setGeometry(QRect(width() * 0.35, height() * 0.35, width() * 0.4, height() * 0.1));
		ProjectSource_Edit->setStyleSheet("QLineEdit{background-color:#3C3C3C;border:1px solid #4A4A4A;border-radius:5px}");
		ProjectSource_Button->setGeometry(QRect(width() * 0.75, height() * 0.36, width() * 0.1, height() * 0.08));
		ProjectSource_Button->setStyleSheet("\
				QPushButton\
					{border:2px solid #777777;border-radius:10px;background-color:#555555;}\
				QPushButton:hover\
					{border:2px solid #777777;border-radius:10px;background-color:rgba(0,0,0,0);}\
				QPushButton:pressed\
					{border:2px solid #777777;border-radius:10px;background-color:#333333;}");
		ProjectCache_Label->setGeometry(QRect(width() * 0.15, height() * 0.5, width() * 0.2, height() * 0.1));
		ProjectCache_Box->setGeometry(QRect(width() * 0.35, height() * 0.5, width() * 0.5, height() * 0.1));
		ProjectCache_Box->setStyleSheet("\
			QComboBox\
				{background-color:#3C3C3C;border:1px solid #4A4A4A;border-radius:5px}\
			QComboBox::drop-down\
				{background-color:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop: 0 #999999,stop: 1.0 #666666);border:1px solid #4A4A4A;border-radius:5px}");
		ProjectThread_Label->setGeometry(QRect(width() * 0.15, height() * 0.65, width() * 0.2, height() * 0.1));
		ProjectThread_Box->setGeometry(QRect(width() * 0.35, height() * 0.65, width() * 0.5, height() * 0.1));
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
                    border-radius:" + QString::number((int)(width() * 0.008)) + "px;\
                }\
                QComboBox QScrollBar::handle:vertical:hover{\
                    background-color:rgba(200,200,200,1);\
                    border-radius:" + QString::number((int)(width() * 0.008)) + "px;\
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
		NoticeLabel->setGeometry(QRect(width() * 0.1, height() * 0.75, width() * 0.8, height() * 0.1));
		StartButton->setGeometry(QRect(width() * 0.75, height() * 0.86, width() * 0.15, height() * 0.07));
		CancelButton->setGeometry(QRect(width() * 0.55, height() * 0.86, width() * 0.15, height() * 0.07));

		StartButton->setStyleSheet("\
				QPushButton\
					{border:2px solid #1473E6;border-radius:10px;background-color:#555555;}\
				QPushButton:hover\
					{border:2px solid #1473E6;border-radius:10px;background-color:rgba(0,0,0,0);}\
				QPushButton:pressed\
					{border:2px solid #1473E6;border-radius:10px;background-color:#333333;}");
		CancelButton->setStyleSheet("\
				QPushButton\
					{border:2px solid #D61921;border-radius:10px;background-color:#555555;}\
				QPushButton:hover\
					{border:2px solid #D61921;border-radius:10px;background-color:rgba(0,0,0,0);}\
				QPushButton:pressed\
					{border:2px solid #D61921;border-radius:10px;background-color:#333333;}");
	}
public slots:
	void createProject() {
		QFile SourceFile(ProjectSource_Edit->text());
		if (ProjectName_Edit->text() != "" && SourceFile.exists() && ProjectCache_Box->currentText() != "" && ProjectThread_Box->currentText() != "") {
			NoticeLabel->setText("正在创建项目...");
			NoticeLabel->repaint();

			QString Settings = "ProjectEdition=1.0\n";
			Settings += "ProjectSource=" + ProjectSource_Edit->text() + "\n";
			if (ProjectCache_Box->currentText() == "持续缓存") { Settings += "ProjectCache=SF\n"; }
			else if (ProjectCache_Box->currentText() == "临时缓存") { Settings += "ProjectCache=ST\n"; }
			Settings += "ProjectStep=0\n";
			Settings += "ProjectThread=" + ProjectThread_Box->currentText() + "\n";

			QDir NewProjectDir;
			NewProjectDir.mkpath("./Users_Data/repos/" + ProjectName_Edit->text());
			QDir ProjectDir = "./Users_Data/repos/" + ProjectName_Edit->text();
			ProjectDir.mkdir("cache");
			ProjectDir.mkdir("output");
			ProjectDir.mkdir("resource");
			QFile SettingsFile;
			SettingsFile.setFileName("./Users_Data/repos/" + ProjectName_Edit->text() + "/Project.ces");
			SettingsFile.open(QIODevice::ReadWrite | QIODevice::Text);
			SettingsFile.write(Settings.toUtf8());
			SettingsFile.close();
			GlobalValue::CurrentProject = ProjectName_Edit->text();
			Sleep(500);
			qApp->exit(1);
		}
		else {
			NoticeLabel->setText("检查是否有空参数或目标文件是否有效！");
		}
		
	}
	void exitPage() {
		qApp->exit(-1);
	}
	void closeEvent(QCloseEvent* event = Q_NULLPTR) {
		qApp->exit(-1);
	}
	void chooseFile() {
		QString ChooseFile = QFileDialog::getOpenFileName(this, "选择视频文件", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "MP4视频文件(*.mp4)");
		if (ChooseFile != "") {
			ProjectSource_Edit->setText(ChooseFile);
		}
	}

};