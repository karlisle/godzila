#pragma once
#ifndef CAPTUREFRAME_H
#define CAPTUREFRAME_H


#include <iostream>
#include <string>
#include <math.h>
#include <iostream>
#include <Windows.h>



#include <opencv2\core\core.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgcodecs\imgcodecs.hpp>

#include "hough.h"

class CaptureFrame {
public:
	CaptureFrame() {
	}
	void capture();
	void hough_circle(cv::Mat image);

private:

};
#endif // !CAPTUREFRAME_H