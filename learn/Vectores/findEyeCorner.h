#pragma once
#ifndef FINDEYECORNER_H
#define FINDEYECORNER_H

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <queue>
#include <stdio.h>
#include <math.h>

class EyeCorner
{
public:
	EyeCorner() {
	}

	void createCornerKernels();
	void releaseCornerKernerl();


private:
	cv::Mat *leftCornerKernel;
	cv::Mat *rightCornerKernel;

	// not constant because stupid opencv type signatures
	float kEyeCornerKernel[4][6] = {
		{ -1,-1,-1, 1, 1, 1 },
		{ -1,-1,-1,-1, 1, 1 },
		{ -1,-1,-1,-1, 0, 3 },
		{ 1, 1, 1, 1, 1, 1 },
	};


};


#endif // !FINDEYECORNER_H