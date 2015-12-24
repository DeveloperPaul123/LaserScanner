#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

#ifndef NO_CAM
VideoCapture cap(0);
#endif

/**
* Main constructor of main window. 
* @param parent the qwidget parent. 
*/
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

/**
* Deconstructor. 
*/
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
* Write data to the serial port. 
* @param data the QString to write to the port. 
*/
void MainWindow::sendSerialData(QString data) {
	if (serialPort->isOpen()) {
		QByteArray arr;
		arr.append(data);
		serialPort->write(arr);
	}
}

/**
* Called when data is ready on the com port from the arduino. 
*/
void MainWindow::onDataReady() {
	if (serialPort->isOpen()) {
		QByteArray arr = serialPort->readAll();
		QString data(arr);
		//TODO: Do something with the data. 
		if (scanning) {
			if (data.compare("D")) {
				// done moving. 
			}
			else if (data.compare("M")) {
				//moving.
			}
			else if (data.compare("Y")) {
				//laser on
			}
			else if (data.compare("U")) {
				//laser off
			}

		}
	}
}

/**
* Called when the move button is clicked. 
*/
void MainWindow::onMoveClicked() {
	QString input = ui->moveLineEdit->text();
	if (input.length() > 0) {
		int steps = std::stoi(input.toStdString());
		moveMotor(steps);
		/*if (serialPort->isOpen()) {
			QByteArray arr;
			arr.append(QString("S%1E").arg(steps));
			serialPort->write(arr);
		}*/
	}
}

/**
* Move the motor a given number of steps. 
* @param steps the number of steps.
*/
void MainWindow::moveMotor(int steps) {
	QString data = QString("S%1E").arg(steps);
	sendSerialData(data);
}

/**
* Send the laser a command to toggle. 
*/
void MainWindow::toggleLaser() {
	sendSerialData(QString("L"));
}

/**
* Called when save button clicked. 
*/
void MainWindow::onSaveImageClicked() {
	imwrite("C:\\Users\\Paul\\Desktop\\test.tif", currentImage);
}

void MainWindow::takeLaserImage() {
	laserImage = currentImage.clone();
}

void MainWindow::takeNoLaserImage() {
	noLaserImage = currentImage.clone();
}

/**
* Save the difference image. 
*/
void MainWindow::saveLaserDifferenceImage() {
	if (laserImage.size > 0 && noLaserImage.size > 0) {
		Mat result = ImageUtils::subtractImages(noLaserImage, laserImage);
		QString name = QString("scan%1.tif").arg(loopCount);
		//save the image. 
		//TODO: Save in the current "scan" directory.
		imwrite(name.toStdString(), result);

		//copy result.
		Mat det;
		cvtColor(result, det, CV_BGR2RGB);
		QImage image((uchar*)det.data, det.cols, det.rows, det.step, QImage::Format_RGB888);
		QGraphicsScene* scene = new QGraphicsScene;
		scene->addPixmap((QPixmap::fromImage(image)));
		//put this in the secondary image. 
		ui->secondaryImage->setScene(scene);
	}
}

/**
* Called when the load image is clicked. 
*/
void MainWindow::onLoadImageClicked() {
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Image"), "*", tr("Image Files (*.png *.jpg *.bmp)"));
}

/**
* Called when the scan button is clicked. 
* Order of scaning loop should be:
*		1) No laser image
*		2) Laser image
*		3) Save differnce image.
*		4) Move to next position -> back to 1)
*/
void MainWindow::onScanClicked() {
	bool ok;
	QStringList *items = new QStringList();
	items->append("180");
	items->append("360");
	items->append("720");
	items->append("1440");
	QString item = QInputDialog::getItem(this, tr("Number of scans"), tr("Number of scans"), *items, 0, false, &ok);
	if (ok) {
		//set loop count and start the looping. 
		loopCount = item.toInt();
	}
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