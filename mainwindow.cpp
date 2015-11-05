#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

VideoCapture cap(0);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	cameraTimer = new QTimer(this);
	connect(cameraTimer, SIGNAL(timeout()), this, SLOT(updatePicture()));
	connect(ui->connectToArduinoButton, SIGNAL(clicked()), this, SLOT(onConnectClicked()));
	lookForSerialPorts();
	serialPort = new QSerialPort(this);

	cameraTimer->start(40);
}

MainWindow::~MainWindow()
{
	cameraTimer->stop();
	cap.release();
    delete ui;
}

/**
* Updates the main image in the gui. 
*/
void MainWindow::updatePicture() {
	cap.read(currentImage);
	Mat det;
	cvtColor(currentImage, det, CV_BGR2RGB);
	QImage image((uchar*)det.data, det.cols, det.rows, det.step, QImage::Format_RGB888);
	QGraphicsScene* scene = new QGraphicsScene;
	scene->addPixmap((QPixmap::fromImage(image)));
	ui->mainImage->setScene(scene);

	QImage smaller = image.scaled(320, 240, Qt::AspectRatioMode::KeepAspectRatio);
	QGraphicsScene *smallScene = new QGraphicsScene;
	smallScene->addPixmap((QPixmap::fromImage(smaller)));
	ui->secondaryImage->setScene(smallScene);
}

void MainWindow::onConnectClicked() {
	/*Mat first = imread("C:\\Users\\Paul\\Documents\\C++Projects\\LaserScanner\\data\\firstPic.png");
	Mat second = imread("C:\\Users\\Paul\\Documents\\C++Projects\\LaserScanner\\data\\secondPic.png");

	Mat result = ImageUtils::subtractImages(first, second);
	imshow("Result", result);*/
	
	QString port = ui->comboBox->currentText();
	if (port.compare("No Ports", Qt::CaseInsensitive)) {
		return;
	}
	serialPort->setPortName(port);
	serialPort->setBaudRate(QSerialPort::Baud9600);
	serialPort->setDataBits(QSerialPort::Data8);
	serialPort->setParity(QSerialPort::NoParity);
	serialPort->setFlowControl(QSerialPort::NoFlowControl);
	serialPort->setStopBits(QSerialPort::OneStop);
	serialPort->open(QIODevice::ReadWrite);
	connect(serialPort, SIGNAL(readyRead()), this, SLOT(onDataReady()));
}

void MainWindow::lookForSerialPorts() {
	QStringList list;
	
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
		list.append(info.portName());
	}

	if (list.length() > 0) {
		ui->comboBox->addItems(list);
	}
	else {
		ui->comboBox->addItem("No Ports");
	}
	
}

void MainWindow::onDataReady() {
	if (serialPort->isOpen()) {
		QByteArray arr = serialPort->readAll();
		QString data(arr);
	
	}
}