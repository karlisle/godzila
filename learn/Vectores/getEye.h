#pragma once
#ifndef GETEYE_H
#define GETEYE_H

// Cabeceras estandar
#include <iostream>
#include <Windows.h>
#include <vector>
#include <math.h>
// Cabeceras de OpenCV
#include <opencv2\core\core.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgcodecs\imgcodecs.hpp>
// Cabeceras de Intraface
#include <intraface\FaceAlignment.h>
#include <intraface\XXDescriptor.h>
// Cabeceras Locales

class GetEye
{
public:
	void getEye();

	GetEye();
	~GetEye();

private:

};

GetEye::GetEye()
{
}

GetEye::~GetEye()
{
}



#endif GETEYE_H // !GETEYE_H
