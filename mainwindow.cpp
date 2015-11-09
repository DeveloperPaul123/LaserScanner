#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

#ifndef NO_CAM
VideoCapture cap(0);
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	cameraTimer = new QTimer(this);
	connect(cameraTimer, SIGNAL(timeout()), this, SLOT(updatePicture()));
	connect(ui->connectToArduinoButton, SIGNAL(clicked()), this, SLOT(onConnectClicked()));
	connect(ui->moveButton, SIGNAL(clicked()), this, SLOT(onMoveClicked()));
	connect(ui->saveImageButton, SIGNAL(clicked()), this, SLOT(onSaveImageClicked()));
	connect(ui->loadImageButton, SIGNAL(clicked()), this, SLOT(onLoadImageClicked()));
	connect(ui->scanButton, SIGNAL(clicked()), this, SLOT(onScanClicked()));
	connect(ui->loadScanButton, SIGNAL(clicked()), this, SLOT(onLoadScanClicked()));
	connect(ui->actionCalibration, SIGNAL(triggered()), this, SLOT(onCalibrate()));

	ui->moveLineEdit->setValidator(new QIntValidator(1, 4000, this));
	lookForSerialPorts();
	serialPort = new QSerialPort(this);
	scanning = false;
	cameraTimer->start(40);
}

MainWindow::~MainWindow()
{
	cameraTimer->stop();
#ifndef NO_CAM
	cap.release();
#endif
    delete ui;
}

/**
* Updates the main image in the gui. 
*/
void MainWindow::updatePicture() {
#ifndef NO_CAM
	cap.read(currentImage);
	Mat det;
	cvtColor(currentImage, det, CV_BGR2RGB);
	QImage image((uchar*)det.data, det.cols, det.rows, det.step, QImage::Format_RGB888);
	QGraphicsScene* scene = new QGraphicsScene;
	scene->addPixmap((QPixmap::fromImage(image)));
	ui->mainImage->setScene(scene);

	/*QImage smaller = image.scaled(320, 240, Qt::AspectRatioMode::KeepAspectRatio);
	QGraphicsScene *smallScene = new QGraphicsScene;
	smallScene->addPixmap((QPixmap::fromImage(smaller)));
	ui->secondaryImage->setScene(smallScene);*/
#endif
}

/**
* Called when the connect button is clicked. 
*/
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

/**
* Looks for serial ports and adds their names to the com port
* combo box. 
*/
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

/**
* Called when data is ready on the com port from the arduino. 
*/
void MainWindow::onDataReady() {
	if (serialPort->isOpen()) {
		QByteArray arr = serialPort->readAll();
		QString data(arr);
	}
}

/**
* Called when the move button is clicked. 
*/
void MainWindow::onMoveClicked() {
	QString input = ui->moveLineEdit->text();
	if (input.length() > 0) {
		int steps = std::stoi(input.toStdString());
		if (serialPort->isOpen()) {
			QByteArray arr;
			arr.append(QString("S%1E").arg(steps));
			serialPort->write(arr);
		}
	}
}

void MainWindow::onSaveImageClicked() {
	
}

void MainWindow::onLoadImageClicked() {
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Image"), "*", tr("Image Files (*.png *.jpg *.bmp)"));
}

/**
* Called when the scan button is clicked. 
*/
void MainWindow::onScanClicked() {

}

/**
* Called when the load button is clicked. 
*/
void MainWindow::onLoadScanClicked() {

}

/**
* Called when calibration is selected from the menu. 
*/
void MainWindow::onCalibrate() {
	
}