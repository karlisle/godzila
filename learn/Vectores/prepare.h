#pragma once
#ifndef PREPARE_H
#define PREPARE_H

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <queue>
#include <stdio.h>
#include <math.h>

#include "findEyeCenter.h"
class Prepare 
{
public:
	Prepare() {
	}
	void display(cv::Mat frame, std::vector<cv::Rect> faces, cv::Rect lEye);
private:
	void findEyes(cv::Mat frame, cv::Rect faces, cv::Rect lEye);
	FindEyeCenter findeye;
};


#endif // !PREPARE_H