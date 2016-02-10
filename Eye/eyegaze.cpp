
#include "eyegaze.h"

void EyeGaze::start()
{
	
	cout << "#############################" << endl;
	cout << "Detectando componentes..." << endl;

	if (!face_cascade.load(face_cascade_name))
	{
		cout << "Error loading face cascade, fix path" << endl;
	}

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

	cout << "Ok, ahora vamos a encontrar los ojos..." << endl;

	//cvNamedWindow("Main", 1);

	
	VideoCapture capture;
	capture.open(0);
	Mat frame;

	if ( capture.isOpened() )
	{
		while (true)
		{
			Mat frame;
			Mat frameOrig;


			capture.read(frame);
			Mat gray_frame;
			cvtColor(frame, gray_frame, COLOR_BGR2GRAY);

			
			if (gray_frame.rows == 0 || gray_frame.cols == 0 )
			{
				cout << "Error: No frame!!" << endl;
				break;
			}
			else
			{

				cout << "Ok, there's a frame" << endl;
				detectandDisplay(gray_frame);
			}
			if (waitKey(27) >= 0)
			{
				break;
			}
		imshow("Main", frame);
		}
	}
	cvDestroyAllWindows();
}

void EyeGaze::detectandDisplay(Mat frame)
{
	vector<Rect> faces;
	
	cout << "Ok, there's a frame" << endl;
	face_cascade.detectMultiScale(frame, faces, 1.1, 2, 0, Size(30, 30));	
}