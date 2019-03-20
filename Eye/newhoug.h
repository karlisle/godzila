#pragma once
#ifndef NEWHOUGH_H
#define NEWHOUGH_H
// Cabeceras de la libreria estandar
#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>
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

class NewHough
{
public:
	enum OPTIONS {
		NO_GRADIENT = 0x1,
		QUANTIZED_GRADIENT = 0x2,
		GRADIENT = 0x4,
		EDGE_WEIGHTING = 0x8
	};
	// constructor de la clase
	NewHough()
	{
		
	}
	
};
#endif