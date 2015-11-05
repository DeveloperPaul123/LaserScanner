#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <opencv2\core\core.hpp>
#include "opencv2\highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "cv.h"

using namespace cv;

class ImageUtils {
public:
	ImageUtils();
	static Mat subtractImages(Mat background, Mat foreground);
};
#endif