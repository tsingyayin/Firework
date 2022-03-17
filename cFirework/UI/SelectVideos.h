#pragma once
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "../global_value.h"
#pragma execution_character_set("utf-8")

class SelectVideos :public QWidget
{
	Q_OBJECT
signals:
	void currentList(QString);
	void closed();
public:
	QString RawList;
	QString CurrentList;
	QTextEdit* FileList;
	QPushButton* EnsureButton;
	QPushButton* UndoButton;
	QPushButton* CloseButton;
	QPushButton* LoadButton;
	QPushButton* DeleteButton;
	QLabel* TitleLabel;
	SelectVideos(QString ProjectSource, QWidget* parent = Q_NULLPTR) {
		this->setParent(parent);
		RawList = ProjectSource;
		CurrentList = ProjectSource;
		EnsureButton = new QPushButton(this);
		UndoButton = new QPushButton(this);
		CloseButton = new QPushButton(this);
		LoadButton = new QPushButton(this);
		DeleteButton = new QPushButton(this);
		FileList = new QTextEdit(this);
		TitleLabel = new QLabel(this);
		FileList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		FileList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		connect(DeleteButton, SIGNAL(clicked()), this, SLOT(deleteLine()));
		connect(LoadButton, SIGNAL(clicked()), this, SLOT(addNewFile()));
		connect(UndoButton, SIGNAL(clicked()), this, SLOT(undoAll()));
		connect(EnsureButton, SIGNAL(clicked()), this, SLOT(saveList()));
		connect(CloseButton, SIGNAL(clicked()), this, SLOT(prepareClose()));
		connect(FileList, SIGNAL(textChanged()), this, SLOT(changeCurrent()));
		EnsureButton->setText("保存更改");
		UndoButton->setText("全部撤销");
		CloseButton->setText("关闭页面");
		LoadButton->setText("预览文件");
		DeleteButton->setText("删除该行");
		this->setWindowTitle("项目资源链接编辑器");
		TitleLabel->setText("  编辑");
		refreshList();
		this->setMinimumSize(800, 450);
		resizeEvent();
	}
	
public slots:
	void changeCurrent() {
		CurrentList = FileList->toPlainText().replace("\n","||");
	}
	void resizeEvent(QResizeEvent* event = Q_NULLPTR) {
		this->setStyleSheet("\
			QWidget\
					{background-color:#333333;color:#FFFFFF;font-size:" + QString::number((int)(height() * 0.04)) + "px;font-family:'Microsoft YaHei'}\
			QPushButton\
					{border:2px solid #777777;border-radius:10px;background-color:#555555;}\
				QPushButton:hover\
					{border:2px solid #777777;border-radius:10px;background-color:rgba(0,0,0,0);}\
				QPushButton:pressed\
					{border:2px solid #777777;border-radius:10px;background-color:#333333;}\
			QTextEdit\
					{border:2px solid #777777;border-radius:10px;background-color:#555555;;font-size:" + QString::number((int)(height() * 0.03)) + "px;font-family:'Microsoft YaHei'}\
			QLabel\
					{background-color:#222222;font-size:" + QString::number((int)(height() * 0.05)) + "px}\
			");
		TitleLabel->setGeometry(QRect(width() * 0, height() * 0, width() * 1, height() * 0.1));
		FileList->setGeometry(QRect(width() * 0.02, height() * 0.11, width() * 0.8, height() * 0.86));
		LoadButton->setGeometry(QRect(width() * 0.84, height() * 0.12, width() * 0.14, height() * 0.1));
		DeleteButton->setGeometry(QRect(width() * 0.84, height() * 0.23, width() * 0.14, height() * 0.1));
		UndoButton->setGeometry(QRect(width() * 0.84, height() * 0.38, width() * 0.14, height() * 0.1));
		EnsureButton->setGeometry(QRect(width() * 0.84, height() * 0.73, width() * 0.14, height() * 0.1));
		CloseButton->setGeometry(QRect(width() * 0.84, height() * 0.86, width() * 0.14, height() * 0.1));
	}
	void refreshList() {
		QStringList List = CurrentList.split("||");
		FileList->setText(List.join("\n"));
	}
	bool addNewFile() {
		QString NewFile = QFileDialog::getOpenFileName(this, "新增资源文件", QStandardPaths::writableLocation(QStandardPaths::HomeLocation), "MP4 Files(*.mp4)");
		if (NewFile != "") {
			if (NewFile.replace("\\", "/").contains(QDir::currentPath().replace("\\", "/") + "/Users_Data/repos/" + GlobalValue::CurrentProject + "/resource")) {
				NewFile = "__PROJECT_LOCAL_RESOURCE__/" + NewFile.replace("\\", "/").section("/", -1, -1);
			}
			QStringList List = CurrentList.split("||");
			for (int i = 0; i < List.length(); i++) {
				if (List[i].replace("\\", "/") == NewFile.replace("\\", "/")) {
					QMessageBox msgBox;
					msgBox.setWindowTitle("未能添加文件到项目");
					msgBox.setText("选中的文件已经存在于项目中。");
					msgBox.exec();
					return FALSE;
				}
				if (List[i].replace("\\", "/").section("/", -1, -1) == NewFile.replace("\\", "/").section("/", -1, -1)) {
					QMessageBox msgBox;
					msgBox.setWindowTitle("未能添加文件到项目");
					msgBox.setText("选中的文件具有文件名："+ NewFile.replace("\\", "/").section("/", -1, -1) +"\n与当前项目内的已有文件\n"+ List[i].replace("\\", "/") +"\n的文件名重复。");
					msgBox.exec();
					return FALSE;
				}
				
			}
			CurrentList += "||" + NewFile;
			refreshList();
			return TRUE;
		}
		return FALSE;
	}
	void undoAll() {
		CurrentList = RawList;
		refreshList();
	}
	void deleteLine() {
		QString List = FileList->toPlainText();
		QTextCursor Cursor = FileList->textCursor();
		int CurrentPosition = Cursor.position() - 1;
		int Count = 0;
		int Next = List.length();
		while (TRUE) {
			if (CurrentPosition < 0) { break; }
			if (List[CurrentPosition] == "\n" ) {
				Count++;
			}		
			CurrentPosition--;
		 }

		 qDebug()<<Cursor.position();
		 QStringList RawList = List.split("\n");
		 QString New = "";
		 for (int i = 0; i < RawList.length(); i++) {
			 if (i != Count) {
				 New += RawList[i] + "||";
			 }
		 }
		 New.resize(New.length() - 2);
		 CurrentList = New;
		 refreshList();
	}
	bool saveList() {
		QStringList List = CurrentList.split("||");
		for (int i = 0; i < List.length(); i++) {
			QFile CheckFile;
			CheckFile.setFileName(List[i].replace("\\", "/").replace("__PROJECT_LOCAL_RESOURCE__", QDir::currentPath().replace("\\", "/") + "/Users_Data/repos/" + GlobalValue::CurrentProject + "/resource"));
			if (!CheckFile.exists()) {
				if (List[i] == "") {
					QMessageBox msgBox;
					msgBox.setWindowTitle("未能执行保存");
					msgBox.setText("索引中存在空白行");
					msgBox.exec();
					return FALSE;
				}
				else {
					QMessageBox msgBox;
					msgBox.setWindowTitle("未能执行保存");
					msgBox.setText("目标文件路径\n" + List[i] + "\n未能指向一个有效的路径。");
					msgBox.exec();
					return FALSE;
				}
			}
			for (int j = 0; j < List.length(); j++) {
				if (i != j) {
					if (List[i] == List[j]) {
						QMessageBox msgBox;
						msgBox.setWindowTitle("未能执行保存");
						msgBox.setText("第" + QString::number(i + 1) + "行与第" + QString::number(j + 1) + "行重复的指向了同一个文件");
						msgBox.exec();
						return FALSE;
					}
				}
			}
		}
		emit currentList(CurrentList);
	}
	void prepareClose() {	
		this->close();
	}
	void closeEvent(QCloseEvent* event = Q_NULLPTR) {
		emit closed();
	}
};