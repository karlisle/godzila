
#ifndef INICIALIZAR_H
#define INICIALIZAR_H
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
#include "eyegaze.h"
#include "newhoug.h"

using namespace std;
using namespace cv;

class Inicializar
{
public:
	// Inicializar constructor
	Inicializar() {
	}
	int detect();
private:


};



#endif // !INICIALIZAR_H
