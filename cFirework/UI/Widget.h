#pragma once
#include <QtWidgets>
#include <QtCore>
#include <QtGui>
#include <QtMultimedia>
#include <QVideoWidget>
#include "..\langcontrol.h"

class CMediaPlayer :public QWidget
{
	Q_OBJECT
public:
	
	CMediaPlayer(QWidget* parent = Q_NULLPTR) {

	}
public slots:
	bool eventFilter(QObject* target,QEvent * e) {

	}
	void chooseFile() {

	}
	void setMaxTime(qint64 MaxTime) {

	}
	void setValue(qint64 PositionTime) {

	}
	void changeTime(int Current) {

	}
	
};

