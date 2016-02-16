#pragma once

#include "inicializar.h"

void drawPose(cv::Mat& img, const cv::Mat& rot, float lineL)
{
	int loc[2] = { 70, 70 };
	int thickness = 2;
	int lineType = 8;

	cv::Mat P = (cv::Mat_<float>(3, 4) <<
		0, lineL, 0, 0,
		0, 0, -lineL, 0,
		0, 0, 0, -lineL);
	P = rot.rowRange(0, 2)*P;
	P.row(0) += loc[0];
	P.row(1) += loc[1];
	cv::Point p0(P.at<float>(0, 0), P.at<float>(1, 0));

	line(img, p0, cv::Point(P.at<float>(0, 1), P.at<float>(1, 1)), cv::Scalar(255, 0, 0), thickness, lineType);
	line(img, p0, cv::Point(P.at<float>(0, 2), P.at<float>(1, 2)), cv::Scalar(0, 255, 0), thickness, lineType);
	line(img, p0, cv::Point(P.at<float>(0, 3), P.at<float>(1, 3)), cv::Scalar(0, 0, 255), thickness, lineType);
}

int Inicializar::detect()
{

	char detectionModel[] = "models/DetectionModel-v1.5.bin";
	char trackingModel[] = "models/TrackingModel-v1.10.bin";
	string faceDetectionModel("models/haarcascade_frontalface_alt2.xml");


	cout << "Iniciando componentes y variables del sistema Eye-gaze...!!" << endl;

	INTRAFACE::XXDescriptor xxd(4);
	INTRAFACE::FaceAlignment fa(detectionModel, trackingModel, &xxd);

	if (!fa.Initialized())
	{
		cerr << "FaceAligment cannot be initialized" << endl;
		return -1;
	}

	cv::CascadeClassifier face_cascade;
	if (!face_cascade.load(faceDetectionModel))
	{
		cerr << "Error loading face detection model" << endl;
		return -1;
	}

	// Inicualizar variables para el loop principal
	// Inicializar dispositivo de captura, posteriormente se 
	// seleccionara a partir de un dialogo.

	VideoCapture capture;
	capture.open(0);

	int key = 0;
	bool isDetect = true;
	float score, notFace = 0.3;


	Mat X, X0;
	Mat fOrig;
	vector<Rect> faces;
	vector<Rect> eyes;

	if (capture.isOpened())
	{
		Sleep(1000);
		while (true)
		{
			Mat frame;
			Mat frameOrig;
			capture.read(frameOrig);
			frame = frameOrig.clone();
			//fOrig = frame.clone();
			//Mat gray_frame;
			//cvtColor(frame, gray_frame, COLOR_BGR2GRAY);


			if (frame.rows == 0 || frame.cols == 0)
			{
				cout << "Error: No frame!!" << endl;
				break;
			}
			if (isDetect)
			{
				face_cascade.detectMultiScale(frame, faces, 1.2, 2, 0, Size(40, 40));

				if (faces.empty())
				{
					imshow("Main", frame);
					key = waitKey(5);
					continue;
				}

				if (fa.Detect(frame, faces[0], X0, score) != INTRAFACE::IF_OK)
				{
					break;
				}
				isDetect = false;
			}
			else
			{
				if (fa.Track(frame, X0, X, score))
				{
					break;
				}
				X0 = X;
			}
			if (score < notFace)
			{
				isDetect = true;
			}
			else
			{
				//cout << "Ok, show that!!";
				for (int i = 0; i < faces.size(); i++)
				{
					circle(frame, Point((int)X0.at<float>(0, i), (int)X0.at<float>(1, i)), 5, Scalar(0, 255, 0), -1);
				}
				INTRAFACE::HeadPose hp;
				fa.EstimateHeadPose(X0, hp);
				drawPose(frame, hp.rot, 50);
				cout << ".";
			}

			imshow("Main", frame);

			if (waitKey(27) >= 0)
			{
				break;
			}

		}
	}
	cvDestroyAllWindows();
	capture.release();
	cin.get();
	return 0;
// Fin del metodo
}
