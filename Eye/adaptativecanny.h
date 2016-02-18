#pragma once
#ifndef ADAPTIVECANNY_H
#define ADAPTIVECANNY_H
// Cabeceras de la libreria estandar
#include <vector>
#include <cmath>
#include <iostream>
// Cabeceras de OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class AdaptativeCanny
{
public:
	// constructor de la clase
	AdaptativeCanny()
	{}
	static Mat adaptativeCanny(cv::Mat img, cv::Mat& GX, cv::Mat& GY, cv::Mat& mag, int& maxTH, int& minTH);



private:
	static void selectThreshold(cv::Mat magGrad, double PercentOfPixelsNotEdges, double ThresholdRatio, double& lowThresh, double& highThresh);
};




#endif