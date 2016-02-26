#pragma once
#ifndef NEWHOUGH_H
#define NEWHOUGH_H
// Cabeceras de la libreria estandar
#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
// Cabeceras de la libreria de OpenCV
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#define PI 3.14159265

using namespace std;
using namespace cv;

typedef struct _TPoint5i {
	int x;
	int y;
	int z;
	int w;
	int v;
	_TPoint5i() {};
	_TPoint5i(int _x, int _y, int _z, int _w, int _v) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
		v = _v;
	}
} TPoint5i;

typedef struct _Point4i {
	int i;
	int j;
	int k;
	double val;
	_Point4i(int _i, int _j, int _k, double _val) {
		i = _i;
		j = _j;
		k = _k;
		val = _val;
	}
} Point4i;



typedef struct _TPoint6m {
	int x;
	int y;
	int z;
	int w;
	int v;
	float u;
	_TPoint6m(int _x, int _y, int _z, int _w, int _v, float _u) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
		v = _v;
		u = _u;
	}
} TPoint6m;


// Clase NewHough
class NewHough
{
public:
	vector<vector<TPoint5i>> lutEllipse;

	enum OPTIONS {
		NO_GRADIENT = 0x1,
		QUANTIZED_GRADIENT = 0x2,
		GRADIENT = 0x4,
		EDGE_WEIGHTING = 0x8
	};
	typedef struct _EllipseHough {
		int xc;
		int yc;
		int a;
		int b;
		float rho;
		//float support_ratio;
		_EllipseHough() {};
		_EllipseHough(int _xc, int _yc, int _a, int _b, float _rho) {
			xc = _xc;
			yc = _yc;
			a = _a;
			b = _b;
			rho = _rho;
		}
	} EllipseHough;

	// constructor de la clase
	NewHough()
	{
		// Init general data structures for efficency
		templates = vector<vector<Point2i>>(100, vector<Point2i>()); // Accounting for circle radiuses from 1 to 50 pxl
		vector<int> radiuses(100);
		for (int i = 0; i < 100; ++i)
			radiuses[i] = i + 1;
		for (int i = 0; i < 100; ++i)
			templates[i] = circlepoints(radiuses[i]);
		lut = getLUT(radiuses);

		//lutEllipse = getEllipseLUT(radiuses);
	}
	/**
	* @brief Method for computing a LUT containing the set of template circle points compatible for each gradient direction
	* @param Circles radius
	* @return LUT containing the set of template circle points compatible for each gradient direction
	*/
	vector<vector<Point3i>> getLUT(vector<int> range);

	//vector<vector<TPoint5i>> getEllipseLUT(vector<int> range);

	/**
	* @brief Method for estimating hough circles in the given edge mask
	* @param Binary edge mask
	* @param Circles radius
	* @param Flag for allowing/not allowing circles FIXXXXX
	* @param Flag for enabling/disabling hough accumulator normalisation
	* @param Maximum number of detectable circles
	* @return Vector containing estimated circles centers and radius
	*/
	vector<Vec3i> circle_hough(cv::Mat edges, vector<int> range, bool same = true, bool normalise = true, int npeaks = 10, cv::Mat GX = cv::Mat(), cv::Mat GY = cv::Mat(), cv::Mat mag = cv::Mat(), int options = QUANTIZED_GRADIENT);

	/**
	* @brief Method for returning the hough circles accumulator
	* @return hough circles accumulator
	*/
	inline vector<vector<vector<double>>> getHoughAccumulator() {
		return hAccumulator;
	}

	vector<vector<Point3i>> lut;

	//vector<EllipseHough> ellipse_hough(cv::Mat edges, cv::Mat bw, vector<int> range, cv::Mat GX, cv::Mat GY, cv::Mat mag, bool same = true, bool normalise = true, int npeaks = 10);

private:
	/**
	* @brief Method for extracting hough peaks from the given hough accumulator
	* @param Hough accumulator
	* @param Circles radius
	* @param Max number of extractable peaks
	* @param Hough accumulator margin (respect to the given edge map)
	* @return Vector containing estimated circles centers and radius
	*/
	//vector<Vec3i> circle_houghpeaks(vector<vector<vector<double>>>& H, vector<int> range, int npeaks, int margin);
	
	//vector<EllipseHough> ellipse_houghpeaks(vector<float>& acc, int nrh, int nch, vector<int> range, int npeaks, int margin);

	/**
	* @brief Method for computing a template circle points given its radius
	* @param Circle radius [mm]
	* @return Vector containing circle points coordinates
	*/
	vector<Point2i> circlepoints(int radius);
	vector<vector<vector<double>>> hAccumulator; // Hough circles accumulator

	vector<vector<Point2i>> templates;
	
	
};
#endif