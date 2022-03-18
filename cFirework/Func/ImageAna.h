#pragma once
#include <QtCore>
#include <QtGui>
#include "../global_value.h"

#include <opencv2/opencv.hpp>
#include <cmath>
using namespace std;
class ImageAnalysis :public QObject
{
	Q_OBJECT
public:
	ImageAnalysis() {}
	static bool exportAllFrame() {
		cv::VideoCapture VideoObject(GlobalValue::CurrentVideo.toLocal8Bit().data());
		GlobalValue::CurrentVideoRate = VideoObject.get(cv::CAP_PROP_FPS);
		QDir CacheDir;
		if (!CacheDir.exists("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1))) {
			CacheDir.mkpath("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1));
		}
		if (!CacheDir.exists("./Users_Data/repos/" + GlobalValue::CurrentProject + "/frame/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1))) {
			CacheDir.mkpath("./Users_Data/repos/" + GlobalValue::CurrentProject + "/frame/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1));
		}
		if (!CacheDir.exists("./Users_Data/repos/" + GlobalValue::CurrentProject + "/output/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1))) {
			CacheDir.mkpath("./Users_Data/repos/" + GlobalValue::CurrentProject + "/output/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1));
		}
		if (VideoObject.isOpened()) {
			int CurrentFrame = 1;
			while (TRUE) {
				cv::Mat Frame;
				bool GotIt = VideoObject.read(Frame);
				if (GotIt) {
					QString FrameImageName = "./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "/" + QString::number(CurrentFrame) + ".jpg";
					cv::imwrite(FrameImageName.toLocal8Bit().data(), Frame);
				}
				else {
					break;
				}
				CurrentFrame += 1;
			}
		}
		else {

		}
	}
	static bool getColorListOf(QString ImageDir) {
		GlobalValue::ColorList.clear();
		QImage Image = QImage(ImageDir);
		int X = Image.width();
		int Y = Image.height();
		int rX = GlobalValue::RecGeo[0] * X;
		int rY = GlobalValue::RecGeo[1] * Y;
		int rW = GlobalValue::RecGeo[2] * X;
		int rH = GlobalValue::RecGeo[3] * Y;
		GlobalValue::ColorHSLMin = { 0.5,0.5,0.5};
		GlobalValue::ColorHSLMax = { 0.5,0.5,0.5};
		//GlobalValue::ColorHSLMax = { (float)Image.pixelColor(rX,rY).hslHueF(),(float)Image.pixelColor(rX,rY).hslSaturationF(),(float)Image.pixelColor(rX,rY).lightnessF() };
		//GlobalValue::ColorHSLMin = { (float)Image.pixelColor(rX,rY).hslHueF(),(float)Image.pixelColor(rX,rY).hslSaturationF(),(float)Image.pixelColor(rX,rY).lightnessF() };
		QString CSVString = "H,S,L \n";
		for (int i = rX; i < rX + rW + 1; i++) {
			for (int j = rY; j < rY + rH + 1; j++) {
				QColor Color = Image.pixelColor(i, j);
				GlobalValue::ColorList.append(Color);
				float HueF = Color.hslHueF();
				float SatF = Color.hslSaturationF();
				float LigF = Color.lightnessF();
				CSVString += QString::number(HueF) + "," + QString::number(SatF) + "," + QString::number(LigF) + "\n";
				//float HueF = Color.hsvHueF();
				//float SatF = Color.hsvSaturationF();
				//float LigF = Color.valueF();
				if (HueF < GlobalValue::ColorHSLMin[0]) { GlobalValue::ColorHSLMin[0] = HueF; }
				if (SatF < GlobalValue::ColorHSLMin[1]) { GlobalValue::ColorHSLMin[1] = SatF; }
				if (LigF < GlobalValue::ColorHSLMin[2]) { GlobalValue::ColorHSLMin[2] = LigF; }
				if (HueF > GlobalValue::ColorHSLMax[0]) { GlobalValue::ColorHSLMax[0] = HueF; }
				if (SatF > GlobalValue::ColorHSLMax[1]) { GlobalValue::ColorHSLMax[1] = SatF; }
				if (LigF > GlobalValue::ColorHSLMax[2]) { GlobalValue::ColorHSLMax[2] = LigF; }
			}
		}
		
		

		/*QFile CSVFile;
		CSVFile.setFileName("./OutputData.csv");
		CSVFile.open(QIODevice::ReadWrite | QIODevice::Text);
		CSVFile.write(CSVString.toStdString().c_str());
		CSVFile.close();*/
		return TRUE;
	}
	static bool flattenImage(QString ImageDir) {
		QImage Image = QImage(ImageDir);
		int X = Image.width();
		int Y = Image.height();
		QImage NewImage = QImage(X, Y, QImage::Format_ARGB32);
		for (int i = 0; i < X; i++) {
			for (int j = 0; j < Y; j++) {
				QColor Color = Image.pixelColor(i, j);
				/*if (GlobalValue::ColorList.contains(Color)) {
					NewImage.setPixelColor(i, j, qRgb(255, 255, 255));
				}
				else {
					NewImage.setPixelColor(i, j, qRgb(0, 0, 0));
				}*/
				if (GlobalValue::ColorHSLMax[0] - GlobalValue::ColorHSLMin[0] >= 0.5) {
				//if (TRUE) {
					if ((GlobalValue::ColorHSLMin[0] >= Color.hslHueF() || Color.hslHueF() >= GlobalValue::ColorHSLMax[0])
						//&& GlobalValue::ColorHSLMin[1] <= Color.hslSaturationF() && Color.hslSaturationF() <= GlobalValue::ColorHSLMax[1] 
						&& GlobalValue::ColorHSLMin[2] <= Color.lightnessF() && Color.lightnessF() <= GlobalValue::ColorHSLMax[2]
						){
						NewImage.setPixelColor(i, j, qRgb(255, 255, 255));
					} else {
						NewImage.setPixelColor(i, j, qRgb(0, 0, 0));
					}
				} else {
					if (GlobalValue::ColorHSLMin[0] <= Color.hslHueF() && Color.hslHueF() <= GlobalValue::ColorHSLMax[0]
						//&& GlobalValue::ColorHSLMin[1] <= Color.hslSaturationF() && Color.hslSaturationF() <= GlobalValue::ColorHSLMax[1]
						&& GlobalValue::ColorHSLMin[2] <= Color.lightnessF() && Color.lightnessF() <= GlobalValue::ColorHSLMax[2]
						){
						NewImage.setPixelColor(i, j, qRgb(255, 255, 255));
					} else {
						NewImage.setPixelColor(i, j, qRgb(0, 0, 0));
					}
				}
			}
		}
		NewImage.save("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "CAC" + QString::number(GlobalValue::CurrentSelectFrame) + ".jpg");
		return TRUE;
	}
	static bool giveFlameTrack(QString ImageRDir, QString ImageBDir) {
		cv::Mat PictureR = cv::imread(ImageRDir.toLocal8Bit().data());
		cv::Mat PictureB = cv::imread(ImageBDir.toLocal8Bit().data(), cv::IMREAD_GRAYSCALE);
		cv::Mat ULabel, Stats, Centroids;
		//cv::imshow("PB",PictureB);
		//cv::imshow("PR",PictureR);
		int gY = PictureR.rows;
		int gX = PictureR.cols;
		int Count = cv::connectedComponentsWithStats(PictureB, ULabel, Stats, Centroids);
		for (int i = 1; i < Count; i++) {
			int x = Stats.at<int>(i, 0);
			int y = Stats.at<int>(i, 1);
			int w = Stats.at<int>(i, 2);
			int h = Stats.at<int>(i, 3);
			if (x < gX*GlobalValue::RdGeo[0] && gX * GlobalValue::RdGeo[0] < x + w && y < gY * GlobalValue::RdGeo[1] && gY * GlobalValue::RdGeo[1] < y + h) {
				int cX = Centroids.at<double>(i, 0);
				int cY = Centroids.at<double>(i, 1);
				GlobalValue::CurrentRec = { w,h };
				qDebug() << cX << cY;
				int dX = max(abs(gX * GlobalValue::RdGeo[0] - cX), (float)1.0);
				int dY = max(abs(gY * GlobalValue::RdGeo[1] - cX), (float)1.0);
				double Angle = atan(dY / dX) * 180 / M_PI;
				cv::Rect Rec(x, y, w, h);
				cv::Scalar colorG(0, 255, 0);
				cv::Scalar colorR(0, 0, 255);
				cv::rectangle(PictureR, Rec, colorG, 2);
				cv::line(PictureR, cv::Point(gX * GlobalValue::RdGeo[0], gY * GlobalValue::RdGeo[1]), cv::Point(cX, cY), colorR, 2);
				cv::imwrite(("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "FNL" + QString::number(GlobalValue::CurrentSelectFrame) + ".jpg").toLocal8Bit().data(), PictureR);
				cv::imwrite(("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cover.png").toLocal8Bit().data(), PictureR);
				break;
			}
		}
	}
};

class TrackSingleImage :public QThread
{
	Q_OBJECT
public:
	QString ImageRDir;
	QString ImageBDir;
	QString gPicName;
	TrackSingleImage(QString PicName) {
		ImageRDir = "./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "/" + PicName;
		ImageBDir = "./Users_Data/repos/" + GlobalValue::CurrentProject + "/frame/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "/" + PicName;
		gPicName = PicName;
	}

	void run() {
		cv::Mat PictureR = cv::imread(ImageRDir.toLocal8Bit().data());
		cv::Mat PictureBR = cv::imread(ImageBDir.toLocal8Bit().data(), cv::IMREAD_GRAYSCALE);
		cv::Mat PictureB = PictureBR;
		//cv::blur(PictureBR, PictureB, {3,3});
		cv::Mat ULabel, Stats, Centroids;
		//cv::imshow("PB",PictureB);
		//cv::imshow("PR",PictureR);
		int gY = PictureR.rows;
		int gX = PictureR.cols;
		int Count = cv::connectedComponentsWithStats(PictureB, ULabel, Stats, Centroids);
		for (int i = 1; i < Count; i++) {
			int x = Stats.at<int>(i, 0);
			int y = Stats.at<int>(i, 1);
			int w = Stats.at<int>(i, 2);
			int h = Stats.at<int>(i, 3);
			if (x < gX * GlobalValue::RdGeo[0] && gX * GlobalValue::RdGeo[0] < x + w && y < gY * GlobalValue::RdGeo[1] && gY * GlobalValue::RdGeo[1] < y + h) {
				int cX = Centroids.at<double>(i, 0);
				int cY = Centroids.at<double>(i, 1);
				double dX = abs(gX * GlobalValue::RdGeo[0] - cX);
				double dY = abs(gY * GlobalValue::RdGeo[1] - cY);
				double Angle = atan(dY / dX) * 180 / M_PI;
				cv::Rect Rec(x, y, w, h);
				cv::Scalar colorG(0, 255, 0);
				cv::Scalar colorR(0, 0, 255);
				cv::rectangle(PictureR, Rec, colorG, 2);
				cv::line(PictureR, cv::Point(gX * GlobalValue::RdGeo[0], gY * GlobalValue::RdGeo[1]), cv::Point(cX, cY), colorR, 2);
				GlobalValue::AnaListMutex.lock();
				GlobalValue::AnaList.append({ gPicName.split(".")[0].toFloat(),w * GlobalValue::MpP,h * GlobalValue::MpP,Angle });
				GlobalValue::AnaListMutex.unlock();
			}
			cv::imwrite(("./Users_Data/repos/" + GlobalValue::CurrentProject + "/output/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "/" + gPicName).toLocal8Bit().data(), PictureR);
			
			break;
			
		}
	}
};

class FlattenSingleImage :public QThread
{
	Q_OBJECT
public:
	QString gPicName;
	FlattenSingleImage(QString PicName) {
		gPicName = PicName;
	};
	void run() {
		GlobalValue::ThreadMutex.lock();
		GlobalValue::CurrentAliveThread++;
		GlobalValue::ThreadMutex.unlock();
		QImage Image = QImage("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "/" + gPicName);
		int X = Image.width();
		int Y = Image.height();
		QImage NewImage = QImage(X, Y, QImage::Format_ARGB32);
		for (int i = 0; i < X; i++) {
			for (int j = 0; j < Y; j++) {
				QColor Color = Image.pixelColor(i, j);
				/*if (GlobalValue::ColorList.contains(Color)) {
					NewImage.setPixelColor(i, j, qRgb(255, 255, 255));
				}
				else {
					NewImage.setPixelColor(i, j, qRgb(0, 0, 0));
				}*/
				if (GlobalValue::ColorHSLMax[0] - GlobalValue::ColorHSLMin[0] >= 0.5) {
					//if (TRUE) {
					if ((GlobalValue::ColorHSLMin[0] >= Color.hslHueF() || Color.hslHueF() >= GlobalValue::ColorHSLMax[0])
						//&& GlobalValue::ColorHSLMin[1] <= Color.hslSaturationF() && Color.hslSaturationF() <= GlobalValue::ColorHSLMax[1] 
						&& GlobalValue::ColorHSLMin[2] <= Color.lightnessF() && Color.lightnessF() <= GlobalValue::ColorHSLMax[2]
						) {
						NewImage.setPixelColor(i, j, qRgb(255, 255, 255));
					}
					else {
						NewImage.setPixelColor(i, j, qRgb(0, 0, 0));
					}
				}
				else {
					if (GlobalValue::ColorHSLMin[0] <= Color.hslHueF() && Color.hslHueF() <= GlobalValue::ColorHSLMax[0]
						//&& GlobalValue::ColorHSLMin[1] <= Color.hslSaturationF() && Color.hslSaturationF() <= GlobalValue::ColorHSLMax[1]
						&& GlobalValue::ColorHSLMin[2] <= Color.lightnessF() && Color.lightnessF() <= GlobalValue::ColorHSLMax[2]
						) {
						NewImage.setPixelColor(i, j, qRgb(255, 255, 255));
					}
					else {
						NewImage.setPixelColor(i, j, qRgb(0, 0, 0));
					}
				}
			}
		}
		NewImage.save("./Users_Data/repos/" + GlobalValue::CurrentProject + "/frame/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "/" + gPicName);
		GlobalValue::ThreadMutex.lock();
		GlobalValue::CurrentAliveThread--;
		GlobalValue::ThreadMutex.unlock();
	}
};

class FlattenThread :public QThread
{
	Q_OBJECT
signals:
	void isEnd();
	void progressFloat(QString, float);
public:
	FlattenThread() {};
	QList<FlattenSingleImage*> ThreadList;
	QList<TrackSingleImage*> FrameThreadList;
	bool exportAllFrame() {
		cv::VideoCapture VideoObject(GlobalValue::CurrentVideo.toLocal8Bit().data());
		GlobalValue::CurrentVideoRate = VideoObject.get(cv::CAP_PROP_FPS);
		QDir CacheDir;
		if (!CacheDir.exists("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1))) {
			CacheDir.mkpath("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1));
		}
		if (!CacheDir.exists("./Users_Data/repos/" + GlobalValue::CurrentProject + "/frame/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1))) {
			CacheDir.mkpath("./Users_Data/repos/" + GlobalValue::CurrentProject + "/frame/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1));
		}
		if (!CacheDir.exists("./Users_Data/repos/" + GlobalValue::CurrentProject + "/output/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1))) {
			CacheDir.mkpath("./Users_Data/repos/" + GlobalValue::CurrentProject + "/output/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1));
		}
		if (VideoObject.isOpened()) {
			int CurrentFrame = 1;
			while (TRUE) {
				cv::Mat Frame;
				bool GotIt = VideoObject.read(Frame);
				if (GotIt) {
					emit progressFloat("第1/4步：正在拆分原视频 ", (float)CurrentFrame/GlobalValue::CurrentFrame);
					QString FrameImageName = "./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "/" + QString::number(CurrentFrame) + ".jpg";
					cv::imwrite(FrameImageName.toLocal8Bit().data(), Frame);
				}
				else {
					break;
				}
				CurrentFrame += 1;
			}
		}
		else {

		}
	}
	void run(void) {

		exportAllFrame();

		QDir FileFolder;
		FileFolder.setPath("./Users_Data/repos/" + GlobalValue::CurrentProject + "/cache/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1));
		FileFolder.setFilter(QDir::Files | QDir::NoSymLinks);
		FileFolder.setNameFilters({ "*.jpg" });
		QFileInfoList ImageList = FileFolder.entryInfoList();
		for (int i = 0; i < ImageList.length(); i++) {
			while (TRUE) {
				GlobalValue::ThreadMutex.lock();
				if (GlobalValue::CurrentAliveThread <= GlobalValue::MaxAliveThread) {
					FlattenSingleImage* Thread = new FlattenSingleImage(ImageList[i].fileName());
					ThreadList.append(Thread);
					Thread->start();
					GlobalValue::ThreadMutex.unlock();

					break;
				};
				GlobalValue::ThreadMutex.unlock();
				emit progressFloat("第2/4步：正在分析火焰区域 ", (float)i / ImageList.length());
			}

		}
		for (int i = 0; i < ThreadList.length(); i++) {

			ThreadList[i]->wait();
			ThreadList[i]->deleteLater();
		}
		qDebug() << GlobalValue::CurrentAliveThread;
		QDir FrameFolder;
		FrameFolder.setPath("./Users_Data/repos/" + GlobalValue::CurrentProject + "/frame/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1));
		FrameFolder.setFilter(QDir::Files | QDir::NoSymLinks);
		FrameFolder.setNameFilters({ "*.jpg" });
		QFileInfoList FrameList = FrameFolder.entryInfoList();
		for (int i = 0; i < FrameList.length(); i++) {
			while (TRUE) {
				GlobalValue::ThreadMutex.lock();
				if (GlobalValue::CurrentAliveThread <= GlobalValue::MaxAliveThread) {
					TrackSingleImage* Thread = new TrackSingleImage(FrameList[i].fileName());
					FrameThreadList.append(Thread);
					Thread->start();
					GlobalValue::ThreadMutex.unlock();

					break;
				};
				GlobalValue::ThreadMutex.unlock();
			}
			emit progressFloat("第3/4步：正在计算火焰数据 ", (float)i / FrameList.length());
		}
		for (int i = 0; i < FrameThreadList.length(); i++) {

			FrameThreadList[i]->wait();
			FrameThreadList[i]->deleteLater();
		}
		
		emit progressFloat("第4/4步：正在整理导出数据", 0);
		qSort(GlobalValue::AnaList.begin(), GlobalValue::AnaList.end(),cmp);
		QString CSVString = "Frame,Width,Height,Angle\n";
		for (int i = 0; i < GlobalValue::AnaList.length(); i++) {
			CSVString += QString::number(GlobalValue::AnaList[i][0]) + "," +
				QString::number(GlobalValue::AnaList[i][1]) + "," +
				QString::number(GlobalValue::AnaList[i][2]) + "," +
				QString::number(GlobalValue::AnaList[i][3]) + "\n";
			emit progressFloat("第4/4步：正在整理导出数据", (float)i / GlobalValue::AnaList.length());
		}
		QFile CSVFile;
		CSVFile.setFileName("./Users_Data/repos/" + GlobalValue::CurrentProject + "/output/" + GlobalValue::CurrentVideo.replace("\\", "/").section("/", -1, -1) + "/OutputData.csv");
		CSVFile.open(QIODevice::ReadWrite | QIODevice::Text);
		CSVFile.write(CSVString.toStdString().c_str());
		CSVFile.close();
		emit progressFloat("第4/4步：正在整理导出数据", 1);
		emit isEnd();
	}
	static bool cmp(const QList<double> A, const QList<double> B) {
		//QList<double>* LA = (QList<double>*)A;
		//QList<double>* LB = (QList<double>*)B;
		if (A[0] < B[0])
		{
			return 1;
		}
		else {
			return 0;
		}
	}
};