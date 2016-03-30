#include "findEyeCorner.h"

void EyeCorner::createCornerKernels()
{
	rightCornerKernel = new cv::Mat(4, 6, CV_32F, kEyeCornerKernel);
	leftCornerKernel = new cv::Mat(4, 6, CV_32F);
	// Voltear en horizontal
	cv::flip(*rightCornerKernel, *leftCornerKernel, 1);
}

void EyeCorner::releaseCornerKernerl()
{
	delete leftCornerKernel;
	delete rightCornerKernel;
}