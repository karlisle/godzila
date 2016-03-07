#include "hough.h"

using namespace std;
using namespace cv;

void NewHough::hough_lines()
{
	
	Mat img;
	//img = imread("bridge.jpg", CV_LOAD_IMAGE_COLOR);
	img = imread("build.jpg", CV_LOAD_IMAGE_COLOR);
	
	Mat dest = img.clone();
	Mat gray;
	Mat bordes;
	GaussianBlur(img, gray, Size(9, 9), 3, 3);
	cvtColor(img, gray, COLOR_BGR2GRAY);
	Canny(gray, bordes, 50, 200, 3);

	
	vector<Vec2f> lines;
	HoughLines(bordes, lines, 1, CV_PI/180, 230, 50, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0];
		float theta = lines[i][1];
		// Lineas Horizontales
		//if(theta > CV_PI/180 * 80 && theta < CV_PI / 180 * 100)
		// Lineas Verticales
		//if(theta > CV_PI/180 * 170 || theta < CV_PI/180 * 10)
		//{

			Point pt1, pt2;

			double a = cos(theta);
			double b = sin(theta);

			double x0 = a * rho;
			double y0 = b * rho;

			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(img, pt1, pt2, Scalar(255, 0, 0), 0.1, CV_AA);
		//}
		
	}

	imshow("Bordes", bordes);
	imshow("Original", img);
	//imshow("Gray", gray);
	waitKey();
	
	cvDestroyAllWindows();
}

void NewHough::hough_linesP()
{

	Mat img;
	img = imread("build.jpg", CV_LOAD_IMAGE_COLOR);
	//img = frame;
	Mat dest = img.clone();
	Mat gray;
	Mat bordes;

	cvtColor(img, gray, COLOR_BGR2GRAY);
	GaussianBlur(dest, gray, Size(9, 9), 3, 3);
	Canny(gray, bordes, 100, 200, 3);
	imshow("Frame", bordes);
	
	vector<Vec4i> lines;
	HoughLinesP(bordes, lines, 1, CV_PI/180, 200, 80, 10);

	
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line( dest, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 0.1, CV_AA );
	}
	imshow("Lieneas", dest);
	waitKey();
}

void NewHough::hough_circle(Mat eye)
{
	Mat img;
	img = eye.clone();
	Mat dest = img.clone();

	/*
	resize(img, dest, Size(), 4, 4);
	imwrite("imgResize.bmp", dest);
	img = frame;
	
	
	Mat gray;
	Mat bordes;
	Mat binary;
	cvtColor(dest, gray, COLOR_BGR2GRAY);
	
	
	binary = gray > 128;
	//GaussianBlur(binary, binary, Size(5, 5), 1, 1);
	Canny(binary, bordes, 5, 60, 3);
	imwrite("bordeOjo.bmp", bordes);
	imshow("Frame_bordes", binary);
	*/

	vector<Vec3f> circles;

	HoughCircles(img, circles, HOUGH_GRADIENT, 1, img.rows/10, 200, 100, 0, 0 );
	if (!circles.empty())
	{
		cout << "Circulos detectados!" << endl;
		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			circle(dest, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			circle(dest, center, radius, Scalar(0, 0, 255), 3, 8, 0);
		}
		imshow("Cosos locos", dest);
	}
	else
	{
		cerr << "No hay circulos " << endl;
		("Img", img);
	}
	
	return;
}
