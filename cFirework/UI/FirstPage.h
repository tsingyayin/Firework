#pragma once
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include "../langcontrol.h"
#include "../global_value.h"
#include "../loadsettings.h"
#include "CEWidgets.h"
#pragma execution_character_set("utf-8")

class uNewProjectWidget :public QFrame
{
	Q_OBJECT
public:
	CE::CEMultiButton* NewEmptyButton;
	CE::CEMultiButton* NewDefaultButton;
	uNewProjectWidget(QWidget* parent = Q_NULLPTR) {
		this->setParent(parent);

		NewEmptyButton = new CE::CEMultiButton(CE::TILayout::VTI, this);
		NewEmptyButton->setCentralImage(":/IRC/Icon/EmptyProject.png");
		NewEmptyButton->setTextStyleSheet("QLabel{font-size:__AUTO__;font-family:'Microsoft YaHei';color:#FFFFFF;background-color:rgba(0,0,0,0);}");
		NewEmptyButton->setText("新建空项目");
		NewEmptyButton->setObjectName("NewEmpty");
		NewEmptyButton->setNormalStyleSheet("\
				QFrame#NewEmpty\
					{border:2px solid #4A4A4A;border-radius:10px;background-color:#4A4A4A;}\
				QFrame#NewEmpty:hover\
					{border:2px solid #4A4A4A;border-radius:10px;background-color:rgba(0,0,0,0);}");
		NewEmptyButton->setPressStyleSheet("\
				QFrame#NewEmpty\
					{border:2px solid #4A4A4A;border-radius:10px;background-color:#333333;}");
		NewDefaultButton = new CE::CEMultiButton(CE::TILayout::VTI, this);
		NewDefaultButton->setCentralImage(":/IRC/Icon/DefaultProject.png");
		NewDefaultButton->setTextStyleSheet("QLabel{font-size:__AUTO__;font-family:'Microsoft YaHei';color:#FFFFFF;background-color:rgba(0,0,0,0);}");
		NewDefaultButton->setText("新建范例项目");
		NewDefaultButton->setObjectName("NewDefault");
		NewDefaultButton->setNormalStyleSheet("\
				QFrame#NewDefault\
					{border:2px solid #4A4A4A;border-radius:10px;background-color:#4A4A4A;}\
				QFrame#NewDefault:hover\
					{border:2px solid #4A4A4A;border-radius:10px;background-color:rgba(0,0,0,0);}");
		NewDefaultButton->setPressStyleSheet("\
				QFrame#NewDefault\
					{border:2px solid #4A4A4A;border-radius:10px;background-color:#333333;}");
		resizeEvent();
		connect(NewDefaultButton, SIGNAL(clicked()), this, SLOT(newDefault()));
		connect(NewEmptyButton, SIGNAL(clicked()), this, SLOT(newEmpty()));
	}
	void resizeEvent(QResizeEvent* event = Q_NULLPTR) {
		NewEmptyButton->setGeometry(QRect(width() * 0.05, height() * 0.11, width() * 0.9, height() * 0.35));
		NewDefaultButton->setGeometry(QRect(width() * 0.05, height() * 0.52, width() * 0.9, height() * 0.35));
	}
public slots:
	void newDefault() {
		GlobalValue::NewWhichProject = "Default";
		qApp->exit(0);
	}
	void newEmpty() {
		GlobalValue::NewWhichProject = "Empty";
		qApp->exit(0);
	}
};

class uOpenProjectWidget :public QWidget
{
	Q_OBJECT
public:
	CE::CEButtonGroup* ButtonGroup;
	QScrollBar* ScrollBar;
	QWidget* CentralWidget;

	uOpenProjectWidget(QWidget* parent = Q_NULLPTR) {
		this->setParent(parent);
		ScrollBar = new QScrollBar(Qt::Vertical, this);
		ButtonGroup = new CE::CEButtonGroup(this);
		CentralWidget = new QWidget(this);
		ScrollBar->hide();
		connect(ScrollBar, SIGNAL(valueChanged(int)), this, SLOT(moveCentralWidget(int)));
		spawnList();
		resizeEvent();
	}
	void spawnList() {
		QDir reposDir;
		reposDir.setPath("./Users_Data/repos/");
		reposDir.setFilter(QDir::Dirs);
		QFileInfoList ProjectList = reposDir.entryInfoList(QDir::Dirs,QDir::SortFlag::Time);
		for (int i = 0; i < ProjectList.length(); i++) {
			if (ProjectList[i].fileName() == "." || ProjectList[i].fileName() == "..") { continue; }
			CE::CEMultiButton* ProButton = new CE::CEMultiButton(CE::TILayout::HIT, CentralWidget, TRUE);
			ProButton->setText(ProjectList[i].fileName());
			ProButton->setCentralImage("./Users_Data/repos/"+ProjectList[i].fileName()+"/cover.png");
			ProButton->setObjectName("ProButton");
			ProButton->setFontPercentage(0.3);
			ProButton->setTextAlignment(Qt::AlignVCenter);
			ProButton->setNormalStyleSheet("\
				QFrame#ProButton\
					{border:2px solid #4A4A4A;border-radius:10px;background-color:#4A4A4A;}\
				QFrame#ProButton:hover\
					{border:2px solid #4A4A4A;border-radius:10px;background-color:rgba(0,0,0,0);}");
			ProButton->setPressStyleSheet("\
				QFrame#ProButton\
					{border:2px solid #4A4A4A;border-radius:10px;background-color:#333333;}");
			
			ProButton->setTextStyleSheet("QLabel{font-size:__AUTO__;font-family:'Microsoft YaHei';color:#FFFFFF;background-color:rgba(0,0,0,0);}");
			ButtonGroup->addCEMultiButton(ProButton);
			ProButton->show();
		}
		if (ButtonGroup->length() > 6) {
			ScrollBar->setMaximum(ButtonGroup->length() - 6);
			ScrollBar->show();
		}
		ScrollBar->setMinimum(0);
		ScrollBar->setSingleStep(1);
	}
	void resizeEvent(QResizeEvent* event = Q_NULLPTR) {
		
		CentralWidget->setGeometry(QRect(0, - height() * 0.15 * ScrollBar->value(), width() * 0.98, height() * 0.15 * ButtonGroup->length()));
		ScrollBar->setGeometry(QRect(width() * 0.98, height() * 0.05, width() * 0.016, height() * 0.9));
		ScrollBar->setStyleSheet("\
				QScrollBar:vertical{\
                    background-color:rgba(0,0,0,0);\
                    margin:0px,0px,0px,0px;\
                    padding-top:0px;\
                    padding-bottom:0px;\
                }\
                QScrollBar::handle:vertical{\
                    background-color:rgba(255,255,255,1);\
                    border-radius:" + QString::number((int)(width() * 0.008)) + "px;\
                }\
                QScrollBar::handle:vertical:hover{\
                    background-color:rgba(200,200,200,1);\
                    border-radius:" + QString::number((int)(width() * 0.008)) + "px;\
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
		for (int i = 0; i < ButtonGroup->length(); i++) {
			ButtonGroup->button(i)->setGeometry(QRect(width() * 0.01, height() * 0.01 + height() * 0.15 * i, width() * 0.97, height() * 0.145));
		}
	}
public slots:
	void wheelEvent(QWheelEvent* event) {
		ScrollBar->setValue(ScrollBar->value() - (int)(event->angleDelta().y() / 120));
	}
	void moveCentralWidget(int value) {
		CentralWidget->setGeometry(QRect(0, - height() * 0.15 * value, width() * 0.98, height() * 0.15 * ButtonGroup->length()));
	}
};

class uTLDRWidget :public QFrame
{
	Q_OBJECT
public:
	QLabel* ProjectName;
	QLabel* ProjectCover;
	QLabel* ProjectTime;
	QLabel* ProjectCache;
	QLabel* ProjectSource;
	QLabel* ProjectStep;
	QLabel* ProjectSize;
	uTLDRWidget(QWidget* parent = Q_NULLPTR) {
		this->setParent(parent);

		ProjectName = new QLabel(this);
		ProjectCover = new QLabel(this);
		ProjectTime = new QLabel(this);
		ProjectCache = new QLabel(this);
		ProjectSource = new QLabel(this);
		ProjectStep = new QLabel(this);
		ProjectSize = new QLabel(this);
		ProjectName->setText("项目名称：");
		ProjectCover->setText("选择项目以获取预览");
		ProjectCover->setAlignment(Qt::AlignCenter);
		ProjectTime->setText("修改日期：");
		ProjectCache->setText("缓存管理：");
		ProjectSource->setText("分析源：");
		//ProjectStep->setText("最高步骤：");
		ProjectSize->setText("项目大小：");
		ProjectCover->setStyleSheet("QLabel{border:1px solid white;border-radius:5px;background-color:#000000}");

		resizeEvent();
	}
	void resizeEvent(QResizeEvent* event = Q_NULLPTR) {
		this->setStyleSheet("QFrame{background-color:#333333;font-family:'Microsoft YaHei';font-size:" + QString::number((int)(height() * 0.1)) + "px;color:#FFFFFF;}");
		
		ProjectName->setGeometry(QRect(width() * 0.03, height() * 0.01, width() * 0.99, height() * 0.3));
		ProjectName->setStyleSheet("QLabel{background-color:#333333;font-family:'Microsoft YaHei';font-size:" + QString::number((int)(height() * 0.2)) + "px;color:#FFFFFF;}");
		ProjectCover->setGeometry(QRect((width() * 0.23 - height() * 0.6 / 9 * 16) / 2 + width()*0.03, height() * 0.35, height() * 0.6 / 9 * 16, height() * 0.6));
		
		ProjectTime->setGeometry(QRect(width() * 0.3, height() * 0.35, width() * 0.35, height() * 0.2));
		ProjectSource->setGeometry(QRect(width() * 0.3, height() * 0.55, width() * 0.35, height() * 0.2));
		ProjectCache->setGeometry(QRect(width() * 0.3, height() * 0.75, width() * 0.35, height() * 0.2));
		ProjectStep->setGeometry(QRect(width() * 0.7, height() * 0.35, width() * 0.2, height() * 0.2));
		ProjectSize->setGeometry(QRect(width() * 0.7, height() * 0.55, width() * 0.2, height() * 0.2));
	}
public slots:
	void loadInfo(QString projectName) {
		QMap<QString, QString> SettingsList;
		SettingsList["ProjectEdition"] = "";
		SettingsList["ProjectSource"] = "";
		//SettingsList["ProjectStep"] = "";
		SettingsList["ProjectCache"] = "";
		SettingsList["ProjectThread"] = "";
		QFile SettingsFile;
		SettingsFile.setFileName("./Users_Data/repos/" + projectName + "/Project.ces");
		SettingsFile.open(QIODevice::ReadOnly | QIODevice::Text);
		if (SettingsFile.isOpen()) {
			QTextStream SettingsFileText(&SettingsFile);
			SettingsFileText.setCodec("UTF-8");
			QString SingleLine;
			for (;;) {
				SingleLine = SettingsFileText.readLine();
				if (SingleLine[0] == "#" || SingleLine[0] == "\n") { continue; }
				SettingsList[SingleLine.section("=", 0, 0)] = SingleLine.section("=", 1, -1);
				if (SettingsFileText.atEnd()) { break; }
			}
			if (SettingsList["ProjectEdition"] == "") { setDestoryInfo(); }
			else if (SettingsList["ProjectSource"] == "") { setDestoryInfo(); }
			//else if (SettingsList["ProjectStep"] == "") { setDestoryInfo(); }
			else if (SettingsList["ProjectCache"] == "") { setDestoryInfo(); }
			else if (SettingsList["ProjectThread"] == "") { setDestoryInfo(); }
			else {
				QFileInfo FileInfo(SettingsFile);
				QDateTime dateTime = FileInfo.lastModified();
				ProjectName->setText("项目名称：" + projectName);
				ProjectCover->setStyleSheet("QLabel{border:1px solid white;border-radius:5px;background-color:#000000;border-image:url('./Users_Data/repos/" + projectName + "/cover.png');}");
				ProjectCover->setText("");
				ProjectTime->setText("修改日期：" + dateTime.toLocalTime().toString("yyyy年MM月dd日 hh:mm:ss"));
				QString CacheLevel = SettingsList["ProjectCache"];
				if (CacheLevel == "SF") { ProjectCache->setText("缓存管理：持续缓存"); }
				else if (CacheLevel == "ST") { ProjectCache->setText("缓存管理：临时缓存"); }
				QString Resource = SettingsList["ProjectSource"];
				ProjectSource->setText("分析源：已链入" + QString::number(Resource.split("||").length()) + "个");
				//ProjectStep->setText("最高步骤：" + SettingsList["ProjectStep"]);
				qint64 ProjectSizeRAW = sizeOf("./Users_Data/repos/" + projectName);
				QString unit = 'B';
				double curSize = ProjectSizeRAW;
				if (curSize > 1024) {
					curSize /= 1024;
					unit = 'K';
					if (curSize > 1024) {
						curSize /= 1024;
						unit = 'M';
						if (curSize > 1024) {
							curSize /= 1024;
							unit = 'G';
						}
					}
				}
				ProjectSize->setText("项目大小："+QString::number(curSize,'f', 2) + unit);
			}
		}
		else {
			setDestoryInfo();
		}
	}
	void setDestoryInfo() {
		ProjectName->setText("项目CES设置文件不正确，该项目无效");
		ProjectCover->setStyleSheet("QLabel{border:1px solid white;border-radius:5px;background-color:#000000}");
		ProjectCover->setText("无法预览无效项目");
		ProjectTime->setText("修改日期：——");
		ProjectCache->setText("缓存管理：——");
		ProjectSource->setText("分析源：——");
		ProjectStep->setText("最高步骤：——");
		ProjectSize->setText("项目大小度量值：——");
	}
	qint64 sizeOf(QString path)
	{
		QDir dir(path);
		qint64 size = 0;
		// dir.entryInfoList(QDir::Files)返回文件信息
		foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
			//计算文件大小
			size += fileInfo.size();
		// dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)返回所有子目录，并进行过滤
		foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
			//若存在子目录，则递归调用sizeOf函数
			size += sizeOf(path + QDir::separator() + subDir);
		return size;
	}
};

class uFirstPage :public QWidget
{
	Q_OBJECT
private:
	QLabel* WelcomeLabel;
	uNewProjectWidget* NewProjectArea;
	uOpenProjectWidget* OpenArea;
	uTLDRWidget* TLDRArea;
public:

	uFirstPage(QWidget* parent = Q_NULLPTR) {
		if (Program_Settings("Window_SizePolicy") == "F") {
			setWindowState(Qt::WindowMaximized);
		}
		else {
			this->resize(Program_Settings("Window_Size").split("_")[0].toInt(), Program_Settings("Window_Size").split("_")[1].toInt());
		}
		this->setMinimumSize(1366, 768);
		
		this->setStyleSheet("QWidget{background-color:#444444;}");
		this->setWindowTitle("Firework 欢迎页面");
		WelcomeLabel = new QLabel(this);
		WelcomeLabel->setText("  开始...");
		WelcomeLabel->setAlignment(Qt::AlignVCenter);
		NewProjectArea = new uNewProjectWidget(this);
		OpenArea = new uOpenProjectWidget(this);
		TLDRArea = new uTLDRWidget(this);
		connect(OpenArea->ButtonGroup, SIGNAL(selected(QString)), this, SLOT(getSelectProject(QString)));
		connect(OpenArea->ButtonGroup, SIGNAL(refreshed()), this, SLOT(getProjectName()));
		resizeEvent();
		
	}
	
	void resizeEvent(QResizeEvent* event = Q_NULLPTR) {
		WelcomeLabel->setStyleSheet("QLabel{background-color:#333333;color:#FFFFFF;font-size:" + QString::number((int)(height() * 0.05)) + "px;font-family:'Microsoft YaHei'}");
		WelcomeLabel->setGeometry(QRect(width() * 0.0, height() * 0.0, width() * 1.0, height() * 0.1));
		NewProjectArea->setGeometry(QRect(width() * 0.0, height() * 0.1, width() * 0.25, height() * 0.9));
		NewProjectArea->setStyleSheet("QFrame{background-color:#555555;}");
		OpenArea->setGeometry(QRect(width() * 0.25, height() * 0.1, width() * 0.75, height() * 0.7));
		TLDRArea->setGeometry(QRect(width() * 0.25, height() * 0.8, width() * 0.75, height() * 0.2));
		if (width() >= 1366 || height() >= 768) {
			writesettings("Window_Size", QString::number(width()) + "_" + QString::number(height()));
		}
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
public slots:
	void getSelectProject(QString ProjectName) {
		GlobalValue::CurrentProject = ProjectName;
		qApp->exit(1);
	}
	void getProjectName() {
		TLDRArea->loadInfo(OpenArea->ButtonGroup->currentString());
	}
	void closeEvent(QCloseEvent* event) {
		qApp->exit(233);
	}
};