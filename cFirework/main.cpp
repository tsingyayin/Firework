#include <dshow.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include "Func/keyFunction.h"
#include "Func/FrameDelay.h"
#include "UI/MainWindow.h"
#include "UI/FirstPage.h"
#include "UI/NewProjectPage.h"
#include "UI/LoadingPage.h"
#include "global_value.h"
#include "loadsettings.h"
#pragma comment(lib,"strmiids.lib")
#pragma execution_character_set("utf-8")
using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	qRegisterMetaType<QMediaPlayer::State>("QMediaPlayer::State");
	QDir::setCurrent(QString::fromLocal8Bit(argv[0]).section("\\", 0, -2));
	qDebug() << QDir::currentPath();
	QApplication app(argc, argv);
	loadsettings();

	//加载页
	LoadingPage* win = new LoadingPage();
	win->show();
	int i = app.exec();
	win->deleteLater();
	//加载页认为环境正常会令i为0
	if (i == 0) {
		//应用主功能循环
		while (TRUE) {
			if (argc == 1) {
				uFirstPage* win = new uFirstPage();
				win->show();
				i = app.exec();
				win->deleteLater();
			}
			else {
				GlobalValue::CurrentProject = QString::fromLocal8Bit(argv[1]).section("\\", -2, -2);
				i = 1;
			}
			if (i == 0) {
				if (GlobalValue::NewWhichProject == "Empty") {
					uNewProjectPage* win = new uNewProjectPage(uNewProjectPage::InitProject::Empty);
					win->show();
					i = app.exec();
					win->deleteLater();
				}
				else {
					uNewProjectPage* win = new uNewProjectPage(uNewProjectPage::InitProject::Default);
					win->show();
					i = app.exec();
					win->deleteLater();
				}
				
			}
			if (i == 1) {
				MainWindow* win = new MainWindow();
				win->show();
				i = app.exec();
				win->deleteLater();
			}
			if (i == 233) {
				break;
			}
		}
	}
	return 0;
}