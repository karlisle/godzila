#include "prepare.h"
#include "constantes.h"
//#include "findEyeCenter.h"
#include "findEyeCorner.h"

using namespace std;

void Prepare::display(cv::Mat frame, vector<cv::Rect> faces, cv::Rect lEye)
{
	EyeCorner eyeCorner;

	std::string main_window_name = "Capture - Face detection";
	std::string face_window_name = "Capture - Face";
	//-- Mostramos el frame y la 
	//cv::namedWindow(main_window_name, CV_WINDOW_NORMAL);
	//cv::imshow(main_window_name, frame);

	cv::RNG(12345);
	cv::Mat debugImage;
	cv::Mat skinCrCbHist = cv::Mat::zeros(cv::Size(256,256), CV_8UC1);

	cv::Mat frameClone = frame.clone();
	

	eyeCorner.createCornerKernels();
	cv::ellipse(skinCrCbHist, cv::Point(113, 155.6), cv::Size(23.4, 15.2), 43.0, 0.0, 360.0, cv::Scalar(255, 255, 255), -1);
	eyeCorner.releaseCornerKernerl();

	vector < cv::Mat> rgbChannels(3);
	cv::split(frameClone, rgbChannels);
	cv::Mat frame_gray = rgbChannels[2];
	//cv::imshow("rgb", frame_gray);
	if (faces.size() > 0)
	{
		this->findEyes(frame, faces[0], lEye);
	}

}

void Prepare::findEyes(cv::Mat frame, cv::Rect face, cv::Rect lEye)
{
	//cv::Mat faceROI = frame_gray(face);
	cv::Mat debugface = frame.clone();

	
	if (kSmoothFaceFactor)
	{
		double sigma = kSmoothFaceFactor * face.width;
		GaussianBlur(frame, frame, cv::Size(3, 3), sigma);

	}
	//-- Encontrar las regiones del ojo y dibujarlas
	/*
	int eye_region_width = face.width *(kEyePercentWidth / 100.0);
	int eye_region_height = face.height *(kEyePercentHeight / 100.0);
	int eye_region_top = face.height*(kEyePercentTop / 100.0);
	cv::Rect leftEyeRegion(face.width * (kEyePercentSide/100.0), eye_region_top, eye_region_width, eye_region_height );
	cv::Rect rightEyeregion(face.width - eye_region_width - face.width*(kEyePercentSide / 100.0), eye_region_top, eye_region_width, eye_region_height);
	*/

	//-- encontrara centro del ojo
	//cv::rectangle(frame, lEye, cv::Scalar(12, 0, 178), 1);
	//cv::imshow("Left Eye IntraFace", frame);
	cv::Point leftPupil = findeye.eyeCenter(frame, lEye, "Left Eye");
	//cv::Point rightPupil = findeye.eyeCenter(faceROI, rightEyeregion, "right Eye");

	//cv::namedWindow("Face RoI", CV_WINDOW_AUTOSIZE);
	//cv::imshow("Face RoI", faceROI);
	
	//cout << ".";

}