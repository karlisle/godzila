#pragma once
#ifndef CAPTUREFRAME_H
#define CAPTUREFRAME_H


// Cabeceras de la libreria estandar
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <time.h>
#include <Windows.h>
//Cabeceras de inclusion de OpenCV
//(verificar la ruta del sistema, o en su defecto local)
// en las propiedades del proyecto
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// Librerias de IntraFace
#include <intraface\FaceAlignment.h>
#include <intraface\XXDescriptor.h>
// Cabeceras locales
#include "prepare.h"


class CaptureFrame {
public:
	CaptureFrame() {
	}
	
	Prepare prepare;

	void detect();
	

private:

};
#endif // !CAPTUREFRAME_H