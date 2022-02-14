//LNGFire Kernel Function Module
#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <ctime>
#include <cmath>
#include "langcontrol.h"
#pragma execution_character_set("utf-8")

using namespace cv;
using namespace std;

static double CurrentVideoRate = 0;

class FunctionFrameWork :public QObject {
    Q_OBJECT
signals:
    void generalMessage(QString);
    void generalState(QString, QString);
};

/*
视频抽帧线程类
ExportFrame(VideoName)
视频名称字符串（带格式后缀)
*/

class ExportFrame :public QThread {
public:
    QString gVideoName;
    ExportFrame(QString VideoName,FunctionFrameWork* parent = Q_NULLPTR) {
        gVideoName = VideoName;
        this->setParent(parent);
    }
    void run() {
        cv::VideoCapture VideoObject(("./videos/" + gVideoName).toStdString());
        CurrentVideoRate = VideoObject.get(cv::CAP_PROP_FPS);
        FunctionFrameWork* parentSignals = static_cast<FunctionFrameWork*>(this->parent());
        if (VideoObject.isOpened()) {
            int CurrentFrame = 1;
            emit parentSignals->generalMessage(msg("Function_Filebuild_FrameExport_Start"));
            while (TRUE) {
                cv::Mat Frame;
                bool GotIt = VideoObject.read(Frame);
                if (GotIt) {
                    QString FrameImageName = "./cache/imagexport/" + QString::number(CurrentFrame) + ".png";
                    cv::imwrite(FrameImageName.toStdString(), Frame);
                    emit parentSignals->generalMessage(msg("Function_Filebuild_FrameExport_Done").arg(QString::number(CurrentFrame) + ".png"));
                }
                else {
                    break;
                }
                CurrentFrame += 1;
             }
        }
        else {
            
            emit parentSignals->generalState("OPEN_FILE_EXCEPTION", gVideoName);
            emit parentSignals->generalMessage(msg("Function_Filebuild_FrameExport_End"));
            emit parentSignals->generalState("SUCCESS", "FRAME_EXPORT");
        }
    }
};

/*
    图像二值化线程类
    BinarizateThread(ThresholdInt, parent)
    二值化阈值数值（注意是数值）、父类指针
*/

struct ImageInfo {
    QString ImageName;
    cv::Mat Image;
};

static QMutex ImageListMutex;

static QList<ImageInfo> ImageList;

static QMutex CurrentAliveThreadMutex;

static int CurrentAliveThread;

/*
图像二值化单图像线程
BinarizateSingleImage(picturename, picture, threshold, parent)
图像名称，imread图像，阈值数值，父类指针
鉴于OpenCV愚蠢的IO会造成总阻塞，因此这个线程只负责二值化的多线程处理
打开与保存需要在此线程之外进行
需要同时配合全局变量CurrentAliveThread和ImageList
当一个线程执行完毕，会对CurrentAliveThread减一
并将结果保存在ImageList中，以[图像名称, 图像]的组合存在
*/

class BinarizateSingleImage :public QThread
{
public:
    cv::Mat gImage;
    int gThresholdInt;
    QString gImageName;
    BinarizateSingleImage(QString ImageName, cv::Mat Image, int ThresholdInt) {
        gImageName = ImageName;
        gImage = Image;
        gThresholdInt = ThresholdInt;
    }

    void run(void) {
        
        cv::Mat GrayImage;
        cv::cvtColor(gImage, GrayImage, cv::COLOR_BGR2GRAY);
        cv::Mat OutputImage;
        cv::threshold(GrayImage, OutputImage, gThresholdInt, 255, cv::THRESH_BINARY);
        ImageListMutex.lock();
        ImageList.append({ gImageName, OutputImage });
        ImageListMutex.unlock();
        CurrentAliveThreadMutex.lock();
        CurrentAliveThread -= 1;
        CurrentAliveThreadMutex.unlock();
        this->deleteLater();
    }
};

class BinarizateThread :public QThread {
public:
    int gThresholdInt;
    FunctionFrameWork* parentSignals;
    BinarizateThread(int ThresholdInt, FunctionFrameWork* parent = Q_NULLPTR) {
        this->setParent(parent);
        gThresholdInt = ThresholdInt;
        parentSignals = static_cast<FunctionFrameWork*>(this->parent());
    }
    
    void run(void) {
        QList<BinarizateSingleImage*> ThreadList;
        CurrentAliveThread = 0;
        ImageList.clear();
        emit parentSignals->generalMessage(msg("Function_Filebuild_Binarizating_Start"));
        QDir ImageExportFolder("./cache/imagexport/");
        ImageExportFolder.setFilter(QDir::Files | QDir::NoSymLinks);
        for (int i = 0; i < ImageExportFolder.count(); i++) {
            if (ImageExportFolder.entryList()[i].contains(".png")) {
                ThreadList.append(new BinarizateSingleImage(ImageExportFolder.entryList()[i], cv::imread(("./cache/imagexport/" + ImageExportFolder.entryList()[i]).toStdString().c_str()), gThresholdInt));
                ThreadList[ThreadList.length()-1]->start();
                CurrentAliveThread += 1;
            }
            while (CurrentAliveThread >= 0) {
                Sleep(500);
            }
        }
        for (int i = 0; i < ImageList.length(); i++) {
            cv::imwrite(("./cache/binarization" + ImageList[i].ImageName).toStdString().c_str(), ImageList[i].Image);
            emit parentSignals->generalMessage(msg("Function_Filebuild_Binarizating_Save").arg(ImageList[i].ImageName));
            emit parentSignals->generalState("SUCCESS", "BINARIZATION");
        }
        ThreadList.clear();
        this->deleteLater();
    }
};


/*
图像分析线程
AnalysisImage(VideoName, RectangleGeometryList, PointGeometryList, HorW, Distance, TargetStep, VideoOutput, parent)
原视频名称，长方形设置列表[长, 宽]，点设置列表[横坐标, 纵坐标]，使用高还是宽（H或W），对应距离数值，合成目标（向抽帧合成还是向二值合成），是否视频输出（0或1），父类指针
*/

struct AnalysisInfo {
    cv::Mat Image;
    QString ImageName;
    double LX;
    double Angle;
    double LY;
};

static QMutex DataListMutex;

static QList<AnalysisInfo> DataList;

static QMutex AnalysisAliveThreadMutex;

static int AnalysisAliveThread;

class AnalysisSingleImage :public QThread {
public:
    cv::Mat gPictureBi;
    double gMpP;
    double gPointX;
    double gPointY;
    cv::Mat gPictureR;
    QString gPictureName;
    AnalysisSingleImage(QString PictureName, cv::Mat PictureBi, cv::Mat PictureR, double MpP, double PointX, double PointY) {
        gPictureBi = PictureBi;
        gPictureR = PictureR;
        gPictureName = PictureName;
        gMpP = MpP;
        gPointX = PointX;
        gPointY = PointY;
    }
    void run() {
        cv::Mat ULabel, States, Centroids;
        int Count;
        Count = cv::connectedComponentsWithStats(gPictureBi, ULabel, States, Centroids);
        for (int i = 1; i < Count; i++) {
            int x = ULabel.at<int>(i,0);
            int y = ULabel.at<int>(i,1);
            int w = ULabel.at<int>(i,2);
            int h = ULabel.at<int>(i,3);
            if (x < gPointX && gPointX < x + w && y < gPointY && gPointY < y + h) {
                int cX = Centroids.at<int>(i, 0);
                int cY = Centroids.at<int>(i, 1);
                int dX = max(abs(gPointX - cX), 1.0);
                int dY = max(abs(gPointX - cX), 1.0);
                double Angle = atan(dY / dX) * 180 / M_PI;
                double LX = gMpP * w;
                double LY = gMpP * h;

                cv::Rect Rec(x, y, w, h);
                cv::Scalar colorG(0, 255, 0);
                cv::Scalar colorR(0, 0, 255);
                cv::rectangle(gPictureR, Rec, colorG, 2);
                cv::line(gPictureR, cv::Point(gPointX, gPointY), cv::Point(cX, cY), colorR, 2);
                DataListMutex.lock();
                DataList.append({ gPictureR,gPictureName,LX,Angle,LY });
                DataListMutex.unlock();
            }
        }
        AnalysisAliveThreadMutex.lock();
        AnalysisAliveThread -= 1;
        AnalysisAliveThreadMutex.unlock();
        this->deleteLater();
    }
};

class AnalysisImage :public QThread {
public:
    QString gVideoName;
    QStringList gRectangleGeometryList;
    QStringList gPointGeometryList;
    QString gHorW;
    double gDistance;
    QString gTargetStep;
    bool gVideoOutput;
    double gMpP;
    float gRawHeight;
    float gRawWidth;
    float gPointX;
    float gPointY;
    AnalysisImage(QString VideoName, QStringList RectangleGeometryList, QStringList PointGeometryList, QString HorW, double Distance, bool VideoOutput , FunctionFrameWork * parent = Q_NULLPTR) {
        this->setParent(parent);
        gVideoName = VideoName;
        gVideoOutput = VideoOutput;
        
        
        QDir ImageExportFolder("./cache/binarization/");
        ImageExportFolder.setFilter(QDir::Files | QDir::NoSymLinks);
        for (int i = 0; i < ImageExportFolder.count(); i++) {
            if (ImageExportFolder.entryList()[i].contains(".png")) {
                cv::Mat SampleImage = cv::imread(("./cache/binarization" + ImageExportFolder.entryList()[i]).toStdString().c_str());
                gRawHeight = SampleImage.size().height;
                gRawWidth = SampleImage.size().width;
                break;
            }
        }
        if (HorW == "H") {
            gMpP = (Distance / RectangleGeometryList[0].toFloat()) * (600 / gRawHeight);
        }
        else {
            gMpP = (Distance / RectangleGeometryList[1].toFloat()) * (900 / gRawWidth);   
        }
        gPointX = PointGeometryList[0].toFloat() * (gRawWidth / 900);
        gPointY = PointGeometryList[1].toFloat() * (gRawWidth / 600);
    }
    void run(void) {
        QList<AnalysisSingleImage*> ThreadList;
        AnalysisAliveThread = 0;
        DataList.clear();
        QDir ImageExportFolder("./cache/binarization/");
        ImageExportFolder.setFilter(QDir::Files | QDir::NoSymLinks);
        for (int i = 0; i < ImageExportFolder.count(); i++) {
            if (ImageExportFolder.entryList()[i].contains(".png")) {
                QString CurrentFileName = ImageExportFolder.entryList()[i];
                ThreadList.append(new AnalysisSingleImage(CurrentFileName,cv::imread(("./cache/binarization/"+ CurrentFileName).toStdString().c_str(),cv::IMREAD_GRAYSCALE),cv::imread(("./cache/imagexport/"+CurrentFileName).toStdString().c_str()),gMpP,gPointX,gPointY));
                ThreadList[ThreadList.length() - 1]->start();
                AnalysisAliveThread += 1;
            }
        }
        while (AnalysisAliveThread >= 0) {
            Sleep(500);
        }
        ThreadList.clear();
        QDateTime CurrentTime = QDateTime::currentDateTime();
        QString CurrentTimeString = CurrentTime.toString("yyyy_MM_dd_HH_mm_ss");
        QString CurrentOutputDir = "./resultexport/" + gVideoName + "_" + CurrentTimeString + "/";
        QDir TargetDir;
        TargetDir.mkpath(CurrentOutputDir);
        QString CSVString = "Width,Angle,Height \n";
        for (int i = 0; i < DataList.length(); i++) {
            cv::imwrite((CurrentOutputDir + DataList[i].ImageName).toStdString().c_str(), DataList[i].Image);
            CSVString += QString::number(DataList[i].LX) + "," + QString::number(DataList[i].Angle) + "," + QString::number(DataList[i].LY) + "\n"; 
        }
        QFile CSVFile;
        CSVFile.setFileName(CurrentOutputDir + "OutputData.csv");
        CSVFile.open(QIODevice::ReadWrite | QIODevice::Text);
        CSVFile.write(CSVString.toStdString().c_str());
        CSVFile.close();

        if (gVideoOutput) {
            QString VideoPath = CurrentOutputDir + gVideoName;
            cv::VideoWriter VideoOutput = cv::VideoWriter(VideoPath.toStdString().c_str(), cv::VideoWriter::fourcc('m', 'p', '4', 'v'), CurrentVideoRate, cv::Size(gRawWidth, gRawHeight));
            qSort(DataList.begin(), DataList.end(), [](AnalysisInfo& a, AnalysisInfo& b) {
                return (a.ImageName.split(".")[0].toDouble() < b.ImageName.split(".")[0].toDouble());
                });
            for (int i = 0; i < DataList.length(); i++) {
                VideoOutput.write(DataList[i].Image);
            }
            VideoOutput.release();
            DataList.clear();
        }
    }
};

class FunctionFramework :public FunctionFrameWork {
public:
    FunctionFramework() {

    }
    /*
        关键目录保全函数
        ensureDirPath()
        无参数
        该函数会确保. / videos; . / cache / imagexport; . / cache / binarization; . / resultexport四个目录存在
    */
    void ensureDirPath() {

        QStringList CriticalPathListE({ "./videos", "./cache/imagexport", "./cache/binarization", "./resultexport" });
        qDebug().noquote() << "sysinfo→" + msg("Function_Filecheck_Seaching_Missing");
        int count = 0;
        QDir singleDir;
        for (int i = 0; i < CriticalPathListE.length(); i++) {
            if (!singleDir.exists(CriticalPathListE[i])) {

                qDebug().noquote() << "sysinfo→Directory '" + msg("Function_Filecheck_Builded_Missing").arg(CriticalPathListE[i]) + "'missed.Now rebuilding...";
                singleDir.mkpath(CriticalPathListE[i]);
                count += 1;
            }
        }
        qDebug().noquote() << msg("Function_Filecheck_Run_End");
    }
    /*
        缓存文件清理函数
        deleteCache()
        无参数
        该函数会清除. / cache / imagexport; . / cache / binarization两个目录下的全部内容
    */
    void deleteCache() {

        QStringList CriticalPathListD({ "./cache/imagexport", "./cache/binarization" });
        qDebug().noquote() << "sysinfo→" + msg("Function_Filecheck_Delete_Cache");
        for (int i = 0; i < CriticalPathListD.length(); i++) {
            QDir CurrentCacheFolder(CriticalPathListD[i]);
            CurrentCacheFolder.setFilter(QDir::Files | QDir::NoSymLinks);
            for (int j = 0; j = CurrentCacheFolder.count(); j++) {
                QFile cacheFile(CriticalPathListD[i] + "/" + CurrentCacheFolder.entryList()[j]);
            }
        }
        qDebug().noquote() << "sysinfo→" + msg("Function_Filecheck_Cache_Clean");
    }

    /*
        视频抽帧函数（独立线程型）
        imageExport(VideoName)
        视频名称字符串（带格式后缀）
        该函数根据传入的视频名称，在. / videos目录下打开该视频，并从中抽出帧图像，并放入. / cache / imagexport
        本函数会调用一个独立线程进行抽帧处理。当需要UI实时响应时，需要使用此函数，避免UI阻塞
    */
    void imageExport(QString VideoName) {

        ExportFrame ImageExport(VideoName);
        ImageExport.start();
    }
    /*
        视频抽帧函数（非独立线程型）
        imageExport(VideoName)
        视频名称字符串（带格式后缀）
        该函数根据传入的视频名称，在. / videos目录下打开该视频，并从中抽出帧图像，并放入. / cache / imagexport
        本函数会直接开始抽帧，因此会阻塞UI进程
    */
    bool imageExportUT(QString VideoName) {
        cv::VideoCapture VideoObject(("./videos/" + VideoName).toStdString());
        if (VideoObject.isOpened()) {
            int CurrentFrame = 1;
            while (TRUE) {
                cv::Mat Frame;
                bool GotIt = VideoObject.read(Frame);
                if (GotIt) {
                    QString FrameImageName = "./cache/imagexport/" + QString::number(CurrentFrame) + ".png";
                    cv::imwrite(FrameImageName.toStdString(), Frame);
                    emit generalMessage(msg("Function_Filebuild_Export_Frame").arg(QString::number(CurrentFrame)) + ".png");
                }
                else { break; }
                CurrentFrame += 1;
            }
        }
        else {
            emit generalState("OPEN_FILE_EXCEPTION", VideoName);
            return FALSE;
        }
        emit generalMessage(msg("Function_Filecheck_Run_End"));
        return TRUE;

    }
    /*
       图像二值化函数（独立线程型）
       binarizateImage(ThresholdInt)
       二值化阈值数值（注意是数值）
       该函数按设定的阈值，将抽帧图像二值化，并放入. / cache / binarization
       本函数会调用一个独立线程进行二值化处理。当需要UI实时响应时，需要使用此函数，避免UI阻塞
   */
    void binarizateImage(int ThresholdInt) {
        BinarizateThread binarizateThread(ThresholdInt);
        binarizateThread.start();
    }
    /*
        图像二值化函数（非独立线程型）
        binarizateImage(ThresholdInt)
        二值化阈值数值（注意是数值）
        该函数按设定的阈值，将抽帧图像二值化，并放入. / cache / binarization
        本函数会直接开始处理二值化，因此会阻塞UI进程
    */
    bool binarizateImageUT(int ThresholdInt) {
        QDir ImageExportFolder("./cache/imagexport/");
        ImageExportFolder.setFilter(QDir::Files | QDir::NoSymLinks);
        for (int i = 0; i < ImageExportFolder.count(); i++) {
            QString CurrentFilename = "./cache/imagexport/" + ImageExportFolder.entryList()[i];
            cv::Mat CurrentImage = cv::imread(CurrentFilename.toStdString());
            cv::Mat GrayImage, BinarizatedImage;
            cv::cvtColor(CurrentImage, GrayImage, cv::ColorConversionCodes::COLOR_BGR2GRAY);
            cv::threshold(GrayImage, BinarizatedImage, ThresholdInt, 255, cv::ThresholdTypes::THRESH_BINARY);
            cv::imwrite(("./cache/binarization/" + ImageExportFolder.entryList()[i]).toStdString(), BinarizatedImage);
        }
        return TRUE;
    }
    void analysisImage(QString VideoName, QStringList RectangleGeometryList, QStringList PointGeometryList, QString HorW, double Distance, bool VideoOutput) {
        AnalysisImage AnalysisPicture(VideoName, RectangleGeometryList, PointGeometryList, HorW, Distance, VideoOutput, this);
        AnalysisPicture.start();
    }
};