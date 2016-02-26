#pragma once
#ifndef VECTORLEARN_H
#define VECTORLEARN_H


#include <iostream>
#include <string>
#include <math.h>

#include <opencv2\core\core.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgcodecs\imgcodecs.hpp>



class NewHough {
public:
	NewHough() {

	}
	Mat src;
	Mat src_gray;
	int thresh = 100;
	int max_thresh = 255;
	

	void hough_lines();
	void hough_linesP();
	void hough_circle();
	void find_contours();
	void find(int, void*);
private:

};
#endif // !VECTORLEARN_H



