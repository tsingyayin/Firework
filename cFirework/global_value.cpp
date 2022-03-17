#include "global_value.h"

QString GlobalValue::CurrentProject = "";
QString GlobalValue::NewWhichProject = "";
QString GlobalValue::CurrentVideo = "";
int GlobalValue::CurrentDuration = 1;
int GlobalValue::CurrentFrame = 1;
int GlobalValue::CurrentPixmapNum = 0;
int GlobalValue::CurrentSelectFrame = 1;
int GlobalValue::loadEditFrame = -1;
QList<float> GlobalValue::RectangleGeo = { 0,0,0,0 };
QList<float> GlobalValue::RoundGeo = { 0,0,0,0 };
QList<float> GlobalValue::RecGeo = { 0,0,0,0 };
QList<float> GlobalValue::RdGeo = { 0,0,0,0 };
QList<float> GlobalValue::ColorHSLMax = { 0,0,0 };
QList<float> GlobalValue::ColorHSLMin = { 0,0,0 };
QList<float> GlobalValue::ColorHSLAvg = { 0,0,0 };
QList<QColor> GlobalValue::ColorList = {};
double GlobalValue::MpP = 0;
int GlobalValue::CurrentAliveThread = 0;
int GlobalValue::MaxAliveThread = 4;
QMutex GlobalValue::ThreadMutex;
double GlobalValue::CurrentVideoRate = 0;
QList<int> GlobalValue::CurrentRec = { 0,0 };
QMutex GlobalValue::AnaListMutex;
QList<QList<double>> GlobalValue::AnaList;