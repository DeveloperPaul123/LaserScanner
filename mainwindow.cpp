#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2\core\core.hpp>
#include "opencv2\highgui.hpp"
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	Mat image;
	image = imread("C:\\Repositories\\MURIGLO\\MuriGlo-Master\\MuriGloLogo.jpg");
	if (image.data) {
		namedWindow("Image");
		imshow("Image", image);
		waitKey(0);
	}
	
}

MainWindow::~MainWindow()
{
    delete ui;
}
