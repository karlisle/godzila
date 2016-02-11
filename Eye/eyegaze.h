#pragma once


#ifndef EYE_CORNER_H
#define EYE_CORNER_H

//#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include <intraface\FaceAlignment.h>
#include <intraface\XXDescriptor.h>

#include <iostream>
#include <queue>
#include <stdio.h>
#include <math.h>
#include <Windows.h>

#include <fstream>


#include "constants.h"
#include "eyegaze.h"
#include "findEyeCenter.h"
#include "findEyeCorner.h"

using namespace std;
using namespace cv;



#define kEyeLeft true
#define kEyeRight false

class EyeGaze
{
public:

	// Constructor
	EyeGaze(){
	}

	void start();
	void detectandDisplay(Mat frame);
	//void releaseCornerKernels();
	//cv::Point2f findEyeCorner(cv::Mat region, bool left, bool left2);
	//cv::Point2f findSubpixelEyeCorner(cv::Mat region, cv::Point maxP);


	string face_cascade_name = "models/haarcascade_frontalface_alt2.xml";
	CascadeClassifier face_cascade;

	string main_window_name = "Capture - Face Detection";
	string face_windows_name = "Capture - Face ";

	//RNG rng(12345);

	Mat debugImage;
	Mat skinCrCbHist = Mat::zeros(Size(256, 256), CV_8UC1);

	// Define required variables and data structures


private:

};



#endif