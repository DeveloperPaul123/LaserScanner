#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qtimer.h"
#include <opencv2\core\core.hpp>
#include "opencv2\highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "cv.h"
#include "ImageUtils.h"
#include "QtSerialPort\qserialport.h"
#include "QtSerialPort\qserialportinfo.h"
#include "QtSerialPort\qtserialportversion.h"
#include "qstring.h"
#include "qiodevice.h"
#include <string>
#include "qfiledialog.h"

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
	void updatePicture();
	void onConnectClicked();
	void onMoveClicked();
	void onSaveImageClicked();
	void onLoadImageClicked();
	void onScanClicked();
	void onLoadScanClicked();
	void onDataReady();

private:
    Ui::MainWindow *ui;
	QTimer *cameraTimer;
	Mat currentImage;
	Mat lastImage;
	void lookForSerialPorts();
	void sendSerialData(QString data);
	QSerialPort *serialPort;
	bool scanning;
};

#endif // MAINWINDOW_H
