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




using namespace std;
using namespace cv;

class FindEyeCenter
{
public:
	FindEyeCenter() {
	}
	Point eyeCenter(Mat eye, Mat face);
private:
};

#endif // !FINDEYECENTER_H
