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
��Ƶ��֡�߳���
ExportFrame(VideoName)
��Ƶ�����ַ���������ʽ��׺)
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
    ͼ���ֵ���߳���
    BinarizateThread(ThresholdInt, parent)
    ��ֵ����ֵ��ֵ��ע������ֵ��������ָ��
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
ͼ���ֵ����ͼ���߳�
BinarizateSingleImage(picturename, picture, threshold, parent)
ͼ�����ƣ�imreadͼ����ֵ��ֵ������ָ��
����OpenCV�޴���IO��������������������߳�ֻ�����ֵ���Ķ��̴߳���
���뱣����Ҫ�ڴ��߳�֮�����
��Ҫͬʱ���ȫ�ֱ���CurrentAliveThread��ImageList
��һ���߳�ִ����ϣ����CurrentAliveThread��һ
�������������ImageList�У���[ͼ������, ͼ��]����ϴ���
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
ͼ������߳�
AnalysisImage(VideoName, RectangleGeometryList, PointGeometryList, HorW, Distance, TargetStep, VideoOutput, parent)
ԭ��Ƶ���ƣ������������б�[��, ��]���������б�[������, ������]��ʹ�ø߻��ǿ�H��W������Ӧ������ֵ���ϳ�Ŀ�꣨���֡�ϳɻ������ֵ�ϳɣ����Ƿ���Ƶ�����0��1��������ָ��
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
        �ؼ�Ŀ¼��ȫ����
        ensureDirPath()
        �޲���
        �ú�����ȷ��. / videos; . / cache / imagexport; . / cache / binarization; . / resultexport�ĸ�Ŀ¼����
    */
    void ensureDirPath() {

        QStringList CriticalPathListE({ "./videos", "./cache/imagexport", "./cache/binarization", "./resultexport" });
        qDebug().noquote() << "sysinfo��" + msg("Function_Filecheck_Seaching_Missing");
        int count = 0;
        QDir singleDir;
        for (int i = 0; i < CriticalPathListE.length(); i++) {
            if (!singleDir.exists(CriticalPathListE[i])) {

                qDebug().noquote() << "sysinfo��Directory '" + msg("Function_Filecheck_Builded_Missing").arg(CriticalPathListE[i]) + "'missed.Now rebuilding...";
                singleDir.mkpath(CriticalPathListE[i]);
                count += 1;
            }
        }
        qDebug().noquote() << msg("Function_Filecheck_Run_End");
    }
    /*
        �����ļ�������
        deleteCache()
        �޲���
        �ú��������. / cache / imagexport; . / cache / binarization����Ŀ¼�µ�ȫ������
    */
    void deleteCache() {

        QStringList CriticalPathListD({ "./cache/imagexport", "./cache/binarization" });
        qDebug().noquote() << "sysinfo��" + msg("Function_Filecheck_Delete_Cache");
        for (int i = 0; i < CriticalPathListD.length(); i++) {
            QDir CurrentCacheFolder(CriticalPathListD[i]);
            CurrentCacheFolder.setFilter(QDir::Files | QDir::NoSymLinks);
            for (int j = 0; j = CurrentCacheFolder.count(); j++) {
                QFile cacheFile(CriticalPathListD[i] + "/" + CurrentCacheFolder.entryList()[j]);
            }
        }
        qDebug().noquote() << "sysinfo��" + msg("Function_Filecheck_Cache_Clean");
    }

    /*
        ��Ƶ��֡�����������߳��ͣ�
        imageExport(VideoName)
        ��Ƶ�����ַ���������ʽ��׺��
        �ú������ݴ������Ƶ���ƣ���. / videosĿ¼�´򿪸���Ƶ�������г��֡ͼ�񣬲�����. / cache / imagexport
        �����������һ�������߳̽��г�֡��������ҪUIʵʱ��Ӧʱ����Ҫʹ�ô˺���������UI����
    */
    void imageExport(QString VideoName) {

        ExportFrame ImageExport(VideoName);
        ImageExport.start();
    }
    /*
        ��Ƶ��֡�������Ƕ����߳��ͣ�
        imageExport(VideoName)
        ��Ƶ�����ַ���������ʽ��׺��
        �ú������ݴ������Ƶ���ƣ���. / videosĿ¼�´򿪸���Ƶ�������г��֡ͼ�񣬲�����. / cache / imagexport
        ��������ֱ�ӿ�ʼ��֡����˻�����UI����
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
       ͼ���ֵ�������������߳��ͣ�
       binarizateImage(ThresholdInt)
       ��ֵ����ֵ��ֵ��ע������ֵ��
       �ú������趨����ֵ������֡ͼ���ֵ����������. / cache / binarization
       �����������һ�������߳̽��ж�ֵ����������ҪUIʵʱ��Ӧʱ����Ҫʹ�ô˺���������UI����
   */
    void binarizateImage(int ThresholdInt) {
        BinarizateThread binarizateThread(ThresholdInt);
        binarizateThread.start();
    }
    /*
        ͼ���ֵ���������Ƕ����߳��ͣ�
        binarizateImage(ThresholdInt)
        ��ֵ����ֵ��ֵ��ע������ֵ��
        �ú������趨����ֵ������֡ͼ���ֵ����������. / cache / binarization
        ��������ֱ�ӿ�ʼ�����ֵ������˻�����UI����
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