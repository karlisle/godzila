#include "test.h"

using namespace std;
void Test::display(cv::Mat frame, std::vector<cv::Rect> faces, cv::Rect lEye, cv::Rect rEye, cv::Point Ell, cv::Point Elr)
//void Test::display()
{
	string wName = "Gaze";
	string nrEye = "R_eye";
	string nlEye = "L_eye";
	cv::namedWindow(wName, CV_WINDOW_AUTOSIZE);
	cv::namedWindow(nlEye, CV_WINDOW_NORMAL);
	cv::namedWindow(nrEye, CV_WINDOW_NORMAL);
	
	cv::Mat faceROI = frame.clone();                                                //-- Hacemos un clon de la imagen que se recive como parametro
	//cv::cvtColor(frame, frame, CV_RGB2GRAY);
	frame.convertTo(faceROI, CV_8U, 2.48, -10.68);									//-- Posterior a eso modificamos el contraste de la imagen 
	
	cv::bilateralFilter(frame, faceROI, 10, 60, 60);
	GaussianBlur(frame, frame, cv::Size(3, 3), 0, 0);

	cv::Mat leye(faceROI(lEye));
	cv::Mat reye(faceROI(rEye));

	//cv::line(faceROI, Ell, Elr,  cv::Scalar(134, 0, 255), 1);
	cv::medianBlur(leye, leye, 5);
	cv::medianBlur(reye, reye, 5);
	
	//cv::bitwise_not(leye, leye);
	//cv::bitwise_not(reye, reye);	


	/*######################################################################*/
	cv::imshow(nlEye, leye);
	cv::imshow(nrEye, reye);
	
	//cv::rectangle(faceROI, lEye, cv::Scalar(0, 255, 0));
	//cv::rectangle(faceROI, rEye, cv::Scalar(0, 255, 0));
	
	cv::imshow(wName, faceROI);
	
}

cv::Mat Test::getSkin(cv::Mat src)
{
	cv::Mat dst = src.clone();
	
	cv::Vec3b cwhite = cv::Vec3b::all(255);
	cv::Vec3b cblack = cv::Vec3b::all(0);

	cv::Mat src_ycrcb, src_hsv;

	cv::cvtColor(src, src_ycrcb, CV_BGR2YCrCb);
	src.convertTo(src_hsv, CV_32FC3);
	cv::cvtColor(src_hsv, src_hsv, CV_BGR2HSV);

	cv::normalize(src_hsv, src_hsv, 0.0, 255.0, cv::NORM_MINMAX, CV_32FC3);

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++) 
		{
			cv::Vec3b pix_bgr = src.ptr<cv::Vec3b>(i)[j];
			int B = pix_bgr.val[0];
			int G = pix_bgr.val[1];
			int R = pix_bgr.val[2];

			bool a = R1(R, G, B);

			cv::Vec3b pix_ycrcb = src_ycrcb.ptr<cv::Vec3b>(i)[j];
			int Y = pix_ycrcb.val[0];
			int Cr = pix_ycrcb.val[1];
			int Cb = pix_ycrcb.val[2];
			bool b = R2(Y, Cr, Cb);

			cv::Vec3b pix_hsv = src_hsv.ptr<cv::Vec3b>(i)[j];
			float H = pix_hsv.val[0];
			float S = pix_hsv.val[1];
			float V = pix_hsv.val[2];
			bool c = R3(H, S, V);
			if (!(a&&b&&c))
			{
				dst.ptr<cv::Vec3b>(i)[j] = cblack;
			}
		}

	}

	return dst;
}