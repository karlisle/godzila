#include "prepare.h"
#include "constantes.h"
//#include "findEyeCenter.h"
#include "findEyeCorner.h"

using namespace std;

void Prepare::display(cv::Mat frame, vector<cv::Rect> faces, cv::Rect lEye)
{
	EyeCorner eyeCorner;
	cv::Mat flipFrame = frame.clone();
	cv::flip(flipFrame, flipFrame, 1);

	std::string main_window_name = "Capture - Face detection";
	std::string face_window_name = "Capture - Face";
<<<<<<< HEAD
	//-- Mostramos el frame y la 
	//cv::namedWindow(main_window_name, CV_WINDOW_NORMAL);
	//cv::imshow(main_window_name, frame);
=======

	cv::namedWindow(main_window_name, CV_WINDOW_NORMAL);
	cv::imshow(main_window_name, flipFrame);
>>>>>>> cb4025f3347f835083f5b68934855f7eed3af5d2

	cv::RNG(12345);
	cv::Mat debugImage;
	cv::Mat skinCrCbHist = cv::Mat::zeros(cv::Size(256,256), CV_8UC1);

	cv::Mat frameClone = frame.clone();
	cv::flip(frameClone, frameClone, 1);

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
<<<<<<< HEAD
	//cv::Mat faceROI = frame_gray(face);
	cv::Mat debugface = frame.clone();
=======
	cv::Mat faceROI = frame_gray(face);
	cv::Mat debugFace = faceROI;
>>>>>>> cb4025f3347f835083f5b68934855f7eed3af5d2

	
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
<<<<<<< HEAD
	//cv::rectangle(frame, lEye, cv::Scalar(12, 0, 178), 1);
	//cv::imshow("Left Eye IntraFace", frame);
	cv::Point leftPupil = findeye.eyeCenter(frame, lEye, "Left Eye");
	//cv::Point rightPupil = findeye.eyeCenter(faceROI, rightEyeregion, "right Eye");

	//cv::namedWindow("Face RoI", CV_WINDOW_AUTOSIZE);
	//cv::imshow("Face RoI", faceROI);
	
	//cout << ".";
=======

	cv::Point leftPupil = findeye.eyeCenter(faceROI, leftEyeRegion, "Left Eye");
	cv::Point rightPupil = findeye.eyeCenter(faceROI, rightEyeregion, "right Eye");
	//-- Mostramos las coordenadas de la pupila
	cout << setfill('.');
	cout << "Left Pupil" << setw(10) << leftPupil << endl;
	cout << "Right Pupil" << setw(10) << rightPupil << endl;
	cout << endl;
	//cv::namedWindow("Face RoI", CV_WINDOW_AUTOSIZE);
	//cv::imshow("Face RoI", faceROI);
	
	//-- Obtener las esquinas
	cv::Rect leftRightCornerRegion(leftEyeRegion);
	leftRightCornerRegion.width -= leftPupil.x;
	leftRightCornerRegion.x += leftPupil.x;
	leftRightCornerRegion.height /= 2;
	leftRightCornerRegion.y += leftRightCornerRegion.height / 2;

	cv::Rect leftLeftCornerregion(leftEyeRegion);
	leftLeftCornerregion.width = leftPupil.x;
	leftLeftCornerregion.height /= 2;
	leftLeftCornerregion.y += leftLeftCornerregion.height / 2;

	cv::Rect rightLeftCornerRegion(rightEyeregion);
	rightLeftCornerRegion.width = rightPupil.x;
	rightLeftCornerRegion.height /= 2;
	rightLeftCornerRegion.y += rightLeftCornerRegion.height / 2;
	
	cv::Rect rightRightCornerRegion(rightEyeregion);
	rightRightCornerRegion.width -= rightPupil.x;
	rightRightCornerRegion.x += rightPupil.x;
	rightRightCornerRegion.height /= 2;
	rightRightCornerRegion.y += rightRightCornerRegion.height / 2;

	cv::rectangle(debugFace, leftRightCornerRegion, cv::Scalar(255,255,0));
	cv::rectangle(debugFace, leftLeftCornerregion, cv::Scalar(255, 255, 0));
	cv::rectangle(debugFace, rightLeftCornerRegion, cv::Scalar(255, 255, 0));
	cv::rectangle(debugFace, rightRightCornerRegion, cv::Scalar(255, 255, 0));


	cv::imshow("Esquinas", debugFace);
>>>>>>> cb4025f3347f835083f5b68934855f7eed3af5d2

}