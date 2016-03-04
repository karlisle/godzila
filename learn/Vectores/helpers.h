#pragma once
#ifndef HELPERS_H
#define HELPERS_H

#include <opencv2\core\core.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgcodecs\imgcodecs.hpp>

#include <iostream>
#include <queue>
#include <stdio.h>
using namespace cv;

class Helper {
public:
	Helper(){}
	//bool rectInImage(cv::Rect rect, cv::Mat image);
	//bool inMat(cv::Point p, int rows, int cols);
	Mat matrixMagnitude(const Mat &matX, const Mat &matY);
	double computeDynamicThreshold(const Mat &mat, double stdDevFactor);

private:

};
#endif