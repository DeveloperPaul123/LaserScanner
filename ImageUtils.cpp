#include "ImageUtils.h"

Mat ImageUtils::subtractImages(Mat back, Mat fore) {
	Mat grayBack;
	Mat grayFore;
	cvtColor(back, grayBack, COLOR_BGR2GRAY);
	cvtColor(fore, grayFore, COLOR_BGR2GRAY);
	Mat sub;
	absdiff(grayBack, grayFore, sub);
	
	Mat thresh;
	threshold(sub, thresh, 120, 255, THRESH_BINARY);
	return thresh;
}