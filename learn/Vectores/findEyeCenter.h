#pragma once
#ifndef FINDEYECENTER_H
#define FINDEYECENTER_H

#include <iostream>
#include <vector>
#include <string>

//Cabeceras de inclusion de OpenCV
//(verificar la ruta del sistema, o en su defecto local)
// en las propiedades del proyecto
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//-- Cabeceras locales :)!


class FindEyeCenter
{
public:
	FindEyeCenter() {
	}
	cv::Point eyeCenter(cv::Mat face, cv::Rect eye, std::string debugWindow);
	cv::Mat floodKilledges(cv::Mat &mat);
private:
	
};

#endif // !FINDEYECENTER_H
