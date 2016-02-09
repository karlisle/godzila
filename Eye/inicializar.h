#pragma once

// Archivos de cabecera de la libreria estandar de C++
#include <iostream>
// Archivos de cabecera de la libreria OpenCV
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>

// Archivos de cabecera de Intraface

#include <intraface\FaceAlignment.h>
#include <intraface\XXDescriptor.h>

// Espacio de nombres estandar y de OpenCV
using namespace std;
using namespace cv;

class Inicalizar
{
public:
	// Cnonstructor de la clase
	Inicalizar(){}

	void detectFace();
private:

};

void Inicalizar::detectFace()
{

	cout << "#############################" << endl;
	cout << "Detectando componentes..." << endl;

	VideoCapture cap(0);						// Habrir la camara por defecto.
	if (!cap.isOpened())						// Revisamos si fue exitosa
	{
		cout << "ERROR: No se encontro ningun dispositivo" << endl;
		return;
	}
	else
	{
		cout << "Se a detectado un dispositivo de video: " << endl;
	}


	// definir las variables requeridas y a estructura de datos
	char detectionModel[] = "models/DetectionModel-v1.5.bin";
	char trackingModel[] = "models/TrackingModel-v1.10.bin";
	string faceDetectionModel("models/haarcascade_frontalface_alt2");
	VideoWriter video;
	Mat fOrig;
	Mat lEye;
	Mat rEye;
	Mat lEyeBW;
	Mat rEyeBW;
	Mat leftEyeBW;
	Mat rightEyeBW;
	Vec3i lCircle;
	Vec3i rCircle;
	//ofstream out(outputName);
	int fnumber = 0;
	int width = (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	// Fuente de video correctamente abirta
	// Inicializar un objeto de XXDescriptor
	INTRAFACE::XXDescriptor xxd(4);
	// Inicilaizar un objeto de FaceAlignment
	//INTRAFACE::FaceAlignment fa(detectionModel, trackingModel, &xxd);


	


	namedWindow("Edges", 1);
	while (true)
	{
		Mat cuadro;
		cap >> cuadro;

		imshow("Edges", cuadro);
		// Si la tecla esc es presionada, se termina la ejecucion, al menos por ahora
		if (waitKey(27) >= 0)
		{
			break;
		}
	}
	// Destrumos las ventanas
	cvDestroyAllWindows();


}
