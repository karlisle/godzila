#pragma once
#ifndef TEST_H
#define TEST_H

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>



#include <iostream>
#include <queue>
#include <stdio.h>
#include <math.h>
#include <string>

#include "findEyeCenter.h"

using namespace std;

class Test
{
public:
	Test() {
	}
	void display(cv::Mat frame, std::vector<cv::Rect> faces, cv::Rect lEye, cv::Rect rEye, cv::Point El, cv::Point Er);
	//void display();
private:
	cv::Mat getSkin(cv::Mat face);

	bool R1(int R, int G, int B)
	{
		bool e1 = (R>95) && (G>40) && (B>20) && ((max(R, max(G, B)) - min(R, min(G, B)))>15) && (abs(R - G)>15) && (R>G) && (R>B);
		bool e2 = (R>220) && (G>210) && (B>170) && (abs(R - G) <= 15) && (R>B) && (G>B);
		return (e1 || e2);
	}

	bool R2(float Y, float Cr, float Cb) 
	{
		bool e3 = Cr <= 1.5862*Cb + 20;
		bool e4 = Cr >= 0.3448*Cb + 76.2069;
		bool e5 = Cr >= -4.5652*Cb + 234.5652;
		bool e6 = Cr <= -1.15*Cb + 301.75;
		bool e7 = Cr <= -2.2857*Cb + 432.85;
		return e3 && e4 && e5 && e6 && e7;
	}

	bool R3(float H, float S, float V)
	{
		return (H<25) || (H > 230);
	}

};


#endif // !TEST_H