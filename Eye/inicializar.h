#pragma once


// Archivos de cabecera de la libreria estandar de C++
#include <iostream>
#include <vector>
#include <fstream>

// Archivos de cabecera de la libreria OpenCV
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>

//Cabeceras locales
#include "eyegaze.h"


// Archivos de cabecera de Intraface
#include <intraface\FaceAlignment.h>
#include <intraface\XXDescriptor.h>
//#include <intraface\FaceAlignment.h>



// Espacio de nombres estandar y de OpenCV
using namespace std;
using namespace cv;
// Define required variables and data structures
int frameWidth = 1;
int frameHeight = 1;
int D = 1;
int H = 1;
//NewHough::OPTIONS detectionMode = NewHough::QUANTIZED_GRADIENT;
//Trainer::MODEL gazeModel;
bool maxDensity = false;
bool binaryThresholding = false;
float k[] = { 0, 0, 0,0, 0, 0,0, 0, 1 };
const Mat K(3, 3, CV_32F, (char*)k);
bool blink = false;
bool acquire = false;

//EyeGaze leftGaze, rightGaze;

string videoName;
string outputName;

class Inicalizar
{
public:
	// Cnonstructor de la clase
	Inicalizar(){}

	void detectFace();
private:

};


// dibujar la posicion de la cabeza 
void drawPose(Mat &img, const Mat &rot, float lineL)
{
	int loc[2] = { 70, 70 };
	int thickness = 2;
	int lineType = 8;

	Mat P = (Mat_<float>(3, 4) <<
		0, lineL, 0, 0,
		0, 0, -lineL, 0,
		0, 0, 0, -lineL
		);
	P = rot.rowRange(0, 2) *P;
	P.row(0) += loc[0];
	P.row(0) += loc[1];
	Point p0(P.at < float >(0, 0), P.at <float>(1, 0));

	line(img, p0, Point(P.at<float>(0, 1), P.at<float>(1, 1)), Scalar(255, 0, 0), thickness, lineType);
	line(img, p0, Point(P.at<float>(0, 2), P.at<float>(1, 2)), Scalar(0, 255, 0), thickness, lineType);
	line(img, p0, Point(P.at<float>(0, 3), P.at<float>(1, 3)), Scalar(0, 0, 255), thickness, lineType);
}

bool compareRect(Rect r1, Rect r2) 
{
	return r1.height < r2.height;
}

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
	string faceDetectionModel("models/haarcascade_frontalface_alt2.xml");
	string eyesDetectionModel("models/haarcascade_eye_tree_eyeglasses.xml");
	VideoWriter video;
	bool record = false;
	Mat fOrig;
	Mat lEye;
	Mat rEye;
	Mat lEyeBW;
	Mat rEyeBW;
	Mat leftEyeBW;
	Mat rightEyeBW;
	Vec3i lCircle;
	Vec3i rCircle;
	ofstream out(outputName);
	int fnumber = 0;
	int width = (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	if (record) {
		video = VideoWriter(videoName, CV_FOURCC_PROMPT, 5, Size(width, height), true); // 5fps
		if (!video.isOpened()) {
			cout << "Unable to open output video file or codec not selected!" << endl;
			return;
		}
	}

	// Fuente de video correctamente abirta
	// Inicializar un objeto de XXDescriptor
	INTRAFACE::XXDescriptor xxd(4);
	// Inicilaizar un objeto de FaceAlignment
	INTRAFACE::FaceAlignment fa(detectionModel, trackingModel, &xxd);
	if ( !fa.Initialized() )
	{
		cout << "FaceAligment Error: No se ha podico cargar el modelo de deteccion" << endl;
		return;
	}
	// Cargar el modelo de deteccion de OPenCV
	CascadeClassifier face_cascade;
	CascadeClassifier eye_cascade;

	if  ( !face_cascade.load(faceDetectionModel) )
	{
		cout << "FaceCascade Error: No se ha podico cargar el modelo de deteccion" << endl;
		return;
	}
	if ( !eye_cascade.load(eyesDetectionModel) )
	{
		cout << "EyeCascade Error: No se ha podico cargar el modelo de deteccion" << endl;
		return;
	}

	// Cremos tres ventanas
	cvNamedWindow("Mirada", 1);
	//namedWindow("Ojo_Izquierdo", 1);
	//namedWindow("Ojo_derecho", 1);

	// Mostar hasta que la tecla Esc sea peresionada
	cout << "Presione 'Esc' para terminar" << endl;

	int key = 0;
	bool isDetected = true;
	float score, notFace = 0.3;
	Mat X, X0;
	vector<Rect> faces;
	vector<Rect> eyes;
	
	while (true)
	{
		fnumber++;
		Mat frame;
		Mat frameOrig;
		cap >> frameOrig;			// Obtener una captura de la camara


		frame = frameOrig.clone();
		fOrig = frame.clone();

		if (frame.rows == 0 || frame.cols == 0)
		{
			cout << "CAMERA ERROR: No se ha podido obtener la imagen " << endl;
			break;
		}
		else
		{
			face_cascade.detectMultiScale(frame, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(50, 50));

			for (size_t i = 0; i < faces.size(); i++)
			{
				Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
				ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

				
				Mat faceROI = frameOrig(faces[i]);
				eye_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
				for (size_t j = 0; j < eyes.size(); j++)
				{
					Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
					int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
					circle(frame, eye_center, radius, Scalar(255, 0, 0), 4, 8, 0);
				}
			}

			imshow("Mirada", frame);

		}

		// Si la tecla esc es presionada, se termina la ejecucion, al menos por ahora
		if (waitKey(27) >= 0)
		{
			break;
			return;
		}
	}
	// Destrumos las ventanas
	cvDestroyAllWindows();

}


