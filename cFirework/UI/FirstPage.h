#pragma once
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include "../langcontrol.h"
#include "../global_value.h"
#include "../loadsettings.h"
#include "CEWidgets.h"
#pragma execution_character_set("utf-8")
class EnsureDelete :public QWidget
{
	Q_OBJECT
signals:
	void isClosed();
public:
	QFrame* CentralWidget;
	QLabel* NoticeLabel;
	QPushButton* YesButton;
	QPushButton* NoButton;
	QGridLayout* CurrentLayout;
	QString ProjectName;
	EnsureDelete(QString ProName, QWidget* parent = Q_NULLPTR) {
		ProjectName = ProName;
		this->setFixedSize(600, 200);
		this->setParent(parent);
		this->setWindowFlag(Qt::FramelessWindowHint);
		this->setAttribute(Qt::WA_TranslucentBackground);
		this->setStyleSheet("QWidget{color:#FFFFFF;font-family:'Microsoft YaHei'};");
		CentralWidget = new QFrame(this);
		CentralWidget->setStyleSheet("QFrame{background-color:#444444;border:3px solid #555555;border-radius:10px;}");
		CentralWidget->setFixedSize(600, 300);
		NoticeLabel = new QLabel(this);
		NoticeLabel->setText("�������ֶ�ɾ����Ŀ����Ϊʹ��Firework����ɾ����Ŀ��������Ŀ���ɻָ���\n��ȷ��Ҫʹ�ó������õ�ɾ��ô��\n�� �� �� �� �� �� ����");
		NoticeLabel->setWordWrap(TRUE);
		YesButton = new QPushButton(this);
		NoButton = new QPushButton(this);
		YesButton->setFixedHeight(30);
		NoButton->setFixedHeight(30);
		connect(YesButton, SIGNAL(clicked()), this, SLOT(isYes()));
		connect(NoButton, SIGNAL(clicked()), this, SLOT(isNo()));
		YesButton->setText("ȷ��");
		NoButton->setText("ȡ��");
		CurrentLayout = new QGridLayout(this);
		CurrentLayout->addWidget(NoticeLabel, 0, 0, 1, 5);
		CurrentLayout->addWidget(NoButton, 1, 1, 1, 1);
		CurrentLayout->addWidget(YesButton, 1, 3, 1, 1);
		this->setLayout(CurrentLayout);
		YesButton->setStyleSheet("\
				QPushButton\
					{border:2px solid #1473E6;border-radius:10px;background-color:#555555;}\
				QPushButton:hover\
					{border:2px solid #1473E6;border-radius:10px;background-color:rgba(0,0,0,0);}\
				QPushButton:pressed\
					{border:2px solid #1473E6;border-radius:10px;background-color:#333333;}");
		NoButton->setStyleSheet("\
				QPushButton\
					{border:2px solid #D61921;border-radius:10px;background-color:#555555;}\
				QPushButton:hover\
					{border:2px solid #D61921;border-radius:10px;background-color:rgba(0,0,0,0);}\
				QPushButton:pressed\
					{border:2px solid #D61921;border-radius:10px;background-color:#333333;}");
	}
public slots:

	void isYes() {
		NoticeLabel->setText("��������ɾ����Ŀ...\n��ȴ�...");
		NoticeLabel->repaint();
		QDir ProjectDir;
		ProjectDir.setPath("./Users_Data/repos/" + ProjectName);
		ProjectDir.removeRecursively();
		emit isClosed();
		this->close();
	}
	void isNo() {

		this->close();
	}
	void closeEvent(QCloseEvent* event) {
		this->hide();
		this->disconnect();
		this->deleteLater();
	}
};

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
		NewEmptyButton->setText("�½�����Ŀ");
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
		NewDefaultButton->setText("�½�������Ŀ");
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
public slots:
	void spawnList() {
		QDir reposDir;
		reposDir.setPath("./Users_Data/repos/");
		reposDir.setFilter(QDir::Dirs);
		QFileInfoList ProjectList = reposDir.entryInfoList(QDir::Dirs,QDir::SortFlag::Time);
		if (!ButtonGroup->isEmpty()) {
			for (int i = 0; i < ButtonGroup->length(); i++) {
				ButtonGroup->button(i)->hide();
				ButtonGroup->button(i)->deleteLater();
			}
		}
		ButtonGroup->clear();
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
		resizeEvent();
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
	void wheelEvent(QWheelEvent* event) {
		if (ButtonGroup->length() > 6) {
			ScrollBar->setValue(ScrollBar->value() - (int)(event->angleDelta().y() / 120));
		}	
	}
	void moveCentralWidget(int value) {
		CentralWidget->setGeometry(QRect(0, - height() * 0.15 * value, width() * 0.98, height() * 0.15 * ButtonGroup->length()));
	}
};

class uTLDRWidget :public QFrame
{
	Q_OBJECT
signals:
	void needRefresh();
public:
	QLabel* ProjectName;
	QLabel* ProjectCover;
	QLabel* ProjectTime;
	QLabel* ProjectCache;
	QLabel* ProjectSource;
	QLabel* ProjectStep;
	QLabel* ProjectSize;
	QPushButton* DeleteButton;
	QString CurrentProject;
	uTLDRWidget(QWidget* parent = Q_NULLPTR) {
		this->setParent(parent);

		ProjectName = new QLabel(this);
		ProjectCover = new QLabel(this);
		ProjectTime = new QLabel(this);
		ProjectCache = new QLabel(this);
		ProjectSource = new QLabel(this);
		ProjectStep = new QLabel(this);
		ProjectSize = new QLabel(this);
		DeleteButton = new QPushButton(this);
		connect(DeleteButton, SIGNAL(clicked()), this, SLOT(ensureDeleteButton()));
		connect(this, SIGNAL(needRefresh()), this, SLOT(initText()));
		initText();
	}
public slots:
	void initText() {
		ProjectName->setText("��Ŀ���ƣ�");
		ProjectCover->setText("ѡ����Ŀ�Ի�ȡԤ��");
		ProjectCover->setAlignment(Qt::AlignCenter);
		ProjectTime->setText("�޸����ڣ�");
		ProjectCache->setText("�������");
		ProjectSource->setText("����Դ��");
		ProjectSize->setText("��Ŀ��С��");
		ProjectCover->setStyleSheet("QLabel{border:1px solid white;border-radius:5px;background-color:#000000}");
		DeleteButton->setText("ɾ������Ŀ");
		DeleteButton->hide();
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
		ProjectSize->setGeometry(QRect(width() * 0.7, height() * 0.35, width() * 0.2, height() * 0.2));

		DeleteButton->setGeometry(QRect(width() * 0.7, height() * 0.60, width() * 0.2, height() * 0.28));
		DeleteButton->setStyleSheet("\
				QPushButton\
					{font-family:'Microsoft YaHei';border:2px solid #D61921;border-radius:10px;background-color:#444444;font-size:" + QString::number((int)(height() * 0.15)) + "px;color:#FFFFFF;}\
				QPushButton:hover\
					{font-family:'Microsoft YaHei';border:2px solid #D61921;border-radius:10px;background-color:#554444;font-size:" + QString::number((int)(height() * 0.15)) + "px;color:#FFFFFF;}\
				QPushButton:pressed\
					{font-family:'Microsoft YaHei';border:2px solid #D61921;border-radius:10px;background-color:#553333;font-size:" + QString::number((int)(height() * 0.15)) + "px;color:#FFFFFF;}");
	}
	void loadInfo(QString projectName) {
		CurrentProject = projectName;
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
				ProjectName->setText("��Ŀ���ƣ�" + projectName);
				ProjectCover->setStyleSheet("QLabel{border:1px solid white;border-radius:5px;background-color:#000000;border-image:url('./Users_Data/repos/" + projectName + "/cover.png');}");
				ProjectCover->setText("");
				ProjectTime->setText("�޸����ڣ�" + dateTime.toLocalTime().toString("yyyy��MM��dd�� hh:mm:ss"));
				QString CacheLevel = SettingsList["ProjectCache"];
				if (CacheLevel == "SF") { ProjectCache->setText("���������������"); }
				else if (CacheLevel == "ST") { ProjectCache->setText("���������ʱ����"); }
				QString Resource = SettingsList["ProjectSource"];
				ProjectSource->setText("����Դ��������" + QString::number(Resource.split("||").length()) + "��");
				//ProjectStep->setText("��߲��裺" + SettingsList["ProjectStep"]);
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
				ProjectSize->setText("��Ŀ��С��"+QString::number(curSize,'f', 2) + unit);
			}
		}
		else {
			setDestoryInfo();
		}
		DeleteButton->show();
	}
	void setDestoryInfo() {
		ProjectName->setText("��ĿCES�����ļ�����ȷ������Ŀ��Ч");
		ProjectCover->setStyleSheet("QLabel{border:1px solid white;border-radius:5px;background-color:#000000}");
		ProjectCover->setText("�޷�Ԥ����Ч��Ŀ");
		ProjectTime->setText("�޸����ڣ�����");
		ProjectCache->setText("�����������");
		ProjectSource->setText("����Դ������");
		ProjectStep->setText("��߲��裺����");
		ProjectSize->setText("��Ŀ��С����ֵ������");
	}
	qint64 sizeOf(QString path)
	{
		QDir dir(path);
		qint64 size = 0;
		// dir.entryInfoList(QDir::Files)�����ļ���Ϣ
		foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
			//�����ļ���С
			size += fileInfo.size();
		// dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)����������Ŀ¼�������й���
		foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
			//��������Ŀ¼����ݹ����sizeOf����
			size += sizeOf(path + QDir::separator() + subDir);
		return size;
	}
	void ensureDeleteButton() {
		EnsureDelete* EnsureDeletePage = new EnsureDelete(CurrentProject);
		connect(EnsureDeletePage, SIGNAL(isClosed()), this, SIGNAL(needRefresh()));
		EnsureDeletePage->setWindowModality(Qt::ApplicationModal);
		EnsureDeletePage->show();
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
		this->setWindowTitle("Firework ��ӭҳ��");
		WelcomeLabel = new QLabel(this);
		WelcomeLabel->setText("  ��ʼ...");
		WelcomeLabel->setAlignment(Qt::AlignVCenter);
		NewProjectArea = new uNewProjectWidget(this);
		OpenArea = new uOpenProjectWidget(this);
		TLDRArea = new uTLDRWidget(this);
		connect(OpenArea->ButtonGroup, SIGNAL(selected(QString)), this, SLOT(getSelectProject(QString)));
		connect(OpenArea->ButtonGroup, SIGNAL(refreshed()), this, SLOT(getProjectName()));
		connect(TLDRArea, SIGNAL(needRefresh()), OpenArea, SLOT(spawnList()));
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