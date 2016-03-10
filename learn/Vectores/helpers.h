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

#include "findEyeCenter.h"

using namespace std;
class Helper {
public:
	Helper(){}
	//bool rectInImage(cv::Rect rect, cv::Mat image);
	//bool inMat(cv::Point p, int rows, int cols);
	cv::Mat matrixMagnitude(const cv::Mat &matX, const cv::Mat &matY);
	double computeDynamicThreshold(const cv::Mat &mat, double stdDevFactor);
	bool inMat(cv::Point p, int rows, int cols);

private:
};
#endif