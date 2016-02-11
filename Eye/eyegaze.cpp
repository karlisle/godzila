
#include "eyegaze.h"

void EyeGaze::start()
{
	
	cout << "***********************";
	cout << "Detectando componentes..." << endl;
	cout << "***********************";

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



	// Define required variables and data structures
	char detectionModel[] = "models/DetectionModel-v1.5.bin";
	char trackingModel[] = "models/TrackingModel-v1.10.bin";
	
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
	int fnumber = 0;
	int width = (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	INTRAFACE::XXDescriptor xxd(4);
	INTRAFACE::FaceAlignment fa(detectionModel, trackingModel, &xxd);

	if ( !fa.Initialized() )
	{
		cout << "ERROR: FaceAligment no se puede inicializar." << endl;
		return;
	}
	
	VideoCapture capture;
	capture.open(0);
	Mat frame;
	bool isDetect = true;
	float score, notFace = 0.3;
	Mat X, X0;
	vector<Rect> faces;

	if ( capture.isOpened() )
	{
		while (true)
		{
			fnumber++;
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
				vector<Rect> rostros;
				cout << "Ok, there's a frame" << endl;
				face_cascade.detectMultiScale(gray_frame, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
				for (size_t i = 0; i < faces.size(); i++)
				{
					Point center( faces[i].x + faces[i].width *0.5, faces[i].y + faces[i].height * 0.5 );
					ellipse(gray_frame, center, Size(faces[i].width * 0.5, faces[i].height * 0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
					

				}
				
			}
			if (waitKey(27) >= 0)
			{
				break;
			}
		imshow("Main", gray_frame);
		}
	}
	cvDestroyAllWindows();
	capture.release();
}

void EyeGaze::detectandDisplay(Mat frame)
{
	
	
	
		
}