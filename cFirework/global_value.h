#pragma once
#include <QtCore>
#include <QtGui>
#pragma execution_character_set("utf-8")
using namespace std;
class GlobalValue
{
public:
	static QString CurrentProject;
	static QString NewWhichProject;
	static QString CurrentVideo;
	static QString PageSize;
	static int CurrentDuration;
	static int CurrentFrame;
	static int CurrentPixmapNum;
	static int CurrentSelectFrame;
	static int loadEditFrame;
	static QList<float> RectangleGeo;
	static QList<float> RoundGeo;
	static QList<float> RecGeo;
	static QList<float> RdGeo;
	static QList<QColor> ColorList;
	static QList<float> ColorHSLMin;
	static QList<float> ColorHSLMax;
	static QList<float> ColorHSLAvg;
	static double MpP;
	static QMutex ThreadMutex;
	static int CurrentAliveThread;
	static int MaxAliveThread;
	static double CurrentVideoRate;
	static QList<int> CurrentRec;
	static QList<QList<double>> AnaList;
	static QMutex AnaListMutex;
};

class GlobalInfoFrame :public QObject
{
	Q_OBJECT
signals:
	void Info(QString);
};
