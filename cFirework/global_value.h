#pragma once
#include <QtCore>
#pragma execution_character_set("utf-8")
class GlobalValue
{
public:
	static QString CurrentProject;
	static QString NewWhichProject;
	static QString CurrentVideo;
	static QString PageSize;
	static int CurrentDuration;
	static int CurrentPixmapNum;
};

class GlobalInfoFrame :public QObject
{
	Q_OBJECT
signals:
	void Info(QString);
};
