#pragma once


#ifndef EYE_CORNER_H
#define EYE_CORNER_H

//#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
// Cabeceras de las libreria estandar
#include<exception>
#include <vector>
// Cabeceras locales
#include "adaptativecanny.h"
#include "newhoug.h"



using namespace std;
using namespace cv;

class ImageTypeExeption : public exception
{
	virtual const char* what() const throw()
	{
		return "Error en el tipo de imagen del ojo: la imagen debe ser BGR";
	}
};
class ImageSizeException : public std::exception
{
	virtual const char* what() const throw()
	{
		return "Imagen del ojo, vacia!";
	}
};

class IrisDetectionException : public std::exception
{
	virtual const char* what() const throw()
	{
		return "contorno del iris no encontrado!";
	}
};

class EyeGaze
{
public:
	// Constructor
	EyeGaze(){
	}
	Vec3i computeIrisCenter(Mat eye, Mat K, float depth, bool thresh, bool maxD, int sColor, int sSpace, int options);

	Point3i computeHeadposition(Mat R, Mat K, Point El, Point Er, int H, int D);

	inline Mat getEqualizedEye()
	{
		return eqEye;
	}


	
private:
	// Define constants
	static const int MIN_IRIS_RADIUS = 5; // mm
	static const int MAX_IRIS_RADIUS = 7; // mm

										  // Define variables
	cv::Mat eqEye; // Equalized B/W eye image
	cv::Mat edges; // Eye edges binary map
	std::vector<int> range; // Depth scaled radius range
	NewHough hough;

};
#endif