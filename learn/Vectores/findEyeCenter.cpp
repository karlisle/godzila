#pragma once
#include "findEyeCenter.h"
#include "helpers.h"
#include "constantes.h"

using namespace std;

#pragma mark Helpers

cv::Point unscalePoint(cv::Point p, cv::Rect origSize) {
	float ratio = (((float)kFastEyeWidth) / origSize.width);
	int x = round(p.x / ratio);
	int y = round(p.y / ratio);
	return cv::Point(x, y);
}

void scaleToFastSize(const cv::Mat &src, cv::Mat &dst) {
	cv::resize(src, dst, cv::Size(kFastEyeWidth, (((float)kFastEyeWidth) / src.cols) * src.rows));
}

cv::Mat computeMatXGradient(const cv::Mat &mat)
{
	cv::Mat out(mat.rows, mat.cols, CV_64F);

	for (int y = 0; y < mat.rows; ++y)
	{
		const uchar *Mr = mat.ptr<uchar>(y);
		double *Or = out.ptr<double>(y);

		Or[0] = Mr[1] - Mr[0];
		for (int x = 1; x < mat.cols - 1; ++x)
		{
			Or[x] = (Mr[x + 1] + Mr[x - 1]) / 2.0;
		}
		Or[mat.cols - 1] = Mr[mat.cols - 1] - Mr[mat.cols - 2];
	}
	return out;
}

void testPossibleCentersFormula(int x, int y, const cv::Mat &weight, double gx, double gy, cv::Mat &out) {
	// for all possible centers
	for (int cy = 0; cy < out.rows; ++cy) {
		double *Or = out.ptr<double>(cy);
		const unsigned char *Wr = weight.ptr<unsigned char>(cy);
		for (int cx = 0; cx < out.cols; ++cx) {
			if (x == cx && y == cy) {
				continue;
			}
			// create a vector from the possible center to the gradient origin
			double dx = x - cx;
			double dy = y - cy;
			// normalize d
			double magnitude = sqrt((dx * dx) + (dy * dy));
			dx = dx / magnitude;
			dy = dy / magnitude;
			double dotProduct = dx*gx + dy*gy;
			dotProduct = std::max(0.0, dotProduct);
			// square and multiply by the weight
			if (kEnableWeight) {
				Or[cx] += dotProduct * dotProduct * (Wr[cx] / kWeightDivisor);
			}
			else {
				Or[cx] += dotProduct * dotProduct;
			}
		}
	}
	//cv::imshow("Out", out);
}

cv::Point FindEyeCenter::eyeCenter(cv::Mat face, cv::Rect lEye, std::string debugWindow)
{
	/* TODO: Code
	* Hacemos cosas locas con la imagen para facilitar el procesamiento
	*/
	//
	//cout << "Encontrar el centro del ojo" << endl;

	cv::Mat face_gray;
	cv::cvtColor(face, face_gray, CV_BGR2GRAY);
	cv::Mat eyeROIUnscaled = face_gray(lEye);
	cv::Mat eyeROI;
	
	scaleToFastSize(eyeROIUnscaled, eyeROI);	
	
	// Encontrar el gradiente
	cv::Mat gradientX = computeMatXGradient(eyeROI);
	cv::Mat gradientY = computeMatXGradient(eyeROI.t()).t();
	// Normalizar y umbralizar el gradiente
	// Calcular todas las magnitudes
	Helper help;
	cv::Mat mags = help.matrixMagnitude(gradientX, gradientY);
	//cv::imshow("Mags", eyeROI);
	// Calcular el umbral
	double gradientThresh = help.computeDynamicThreshold(mags, kGradientThreshold);
	for (int y = 0; y < eyeROI.rows; ++y) {
		double *Xr = gradientX.ptr<double>(y), *Yr = gradientY.ptr<double>(y);
		const double *Mr = mags.ptr<double>(y);
		for (int x = 0; x < eyeROI.cols; ++x) {
			double gX = Xr[x], gY = Yr[x];
			double magnitude = Mr[x];
			if (magnitude > gradientThresh) {
				Xr[x] = gX / magnitude;
				Yr[x] = gY / magnitude;
			}
			else {
				Xr[x] = 0.0;
				Yr[x] = 0.0;
			}
		}
	}
	
	//cv::imshow(debugWindow , gradientX);
	//cv::imshow(debugWindow, gradientY);

	//-- Create a blurred and inverted image for weighting 
	cv::Mat weight;
	GaussianBlur(eyeROI, weight, cv::Size(kWeightBlurSize, kWeightBlurSize), 0, 3);
	/*
	for (int y = 0; y < eyeROI.rows; ++y)
	{
		unsigned char *row = weight.ptr<unsigned char>(y);
		for (int x = 0; x < eyeROI.cols; ++x)
		{
			row[x] = (row[x] - 255);
		}
	}
	*/
	cv::Mat hist, image;
	int histSize = 256;
	float range[] = { 0, 255 };
	const float *ranges[] = { range };
	//-- Cacular el histograma
	cv::calcHist(&face_gray, 1, 0, cv::Mat(), hist, 1, &histSize, ranges, true, false );
	//-- Ver el histograma obtenido en la ventana de comandos
	double total;
	total = face_gray.rows * face_gray.cols;
	
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double) hist_w/histSize);
	cv::Mat histImage(hist_h, hist_w, CV_8UC1, cv::Scalar(0, 0, 0));
	cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
	for (int i = 1; i < histSize; i++)
	{
		cv::line(histImage, cv::Point(bin_w *(i-1), hist_h - cvRound(hist.at<float>(i-1)) ), 
							cv::Point(bin_w*(i), hist_h -cvRound(hist.at<float>(i)) ), 
							cv::Scalar(255, 0, 0), 2, 6, 0);
	}
	

	cv::Mat  outSum = cv::Mat::zeros(eyeROI.rows, eyeROI.cols, CV_64F);
	
	printf("Eye Size: %ix%i\n", outSum.cols, outSum.rows);
	for (int y = 0; y < weight.rows; ++y)
	{
		const double *Xr = gradientX.ptr<double>(y), *Yr = gradientY.ptr<double>(y);
		for (int x = 0; x < weight.cols; ++x)
		{
			double gX = Xr[x], gY = Yr[x];
			if (gX == 0.0 && gY == 0.0)
			{
				continue;
			}
			testPossibleCentersFormula(x, y, weight, gX, gY, outSum);
		}
	}

	//--
	double numGradients = (weight.rows * weight.cols);
	cv::Mat out;
	outSum.convertTo(out, CV_32F, 1.0/numGradients);
	//-- Encontrar el punto maximo
	cv::Point maxP;
	double maxVal;
	cv::minMaxLoc(out, NULL, &maxVal, NULL, &maxP);

	//-- Rellenar los bordes
	if (kEnablePostProcess)
	{
		cv::Mat floodClone;
		double floodThresh = maxVal * kPostProcessThreshold;
		cv::threshold(out, floodClone, floodThresh, 0.0f, cv::THRESH_TOZERO);
		//cv::namedWindow("floodClone", CV_WINDOW_NORMAL);
		//cv::imshow("floodClone", floodClone);
		if (kPlotVectorField)
		{
			cv::imwrite("eyeFrame.bmp", eyeROIUnscaled);
		}
		cv::Mat mask = floodKilledges(floodClone);
	}

	//-- Creamos una ventana
	cv::namedWindow(debugWindow, CV_WINDOW_NORMAL);
	// dibujar la region del ojo
	cv::rectangle(face, lEye, 1234);
	//cv::imshow("Face Original", face);

	cv::Point mp = unscalePoint(maxP, lEye);
	cv::rectangle(eyeROI, mp, cv::Point(mp.x + 2, mp.y - 2), cv::Scalar(255, 0, 0), 2, 0);

	cv::imshow(debugWindow, eyeROI);
	cv::imshow("Face Original", face);
	
	return unscalePoint(maxP, lEye);


}
#pragma mark Postprocessing





bool floodShouldpushPoint(const cv::Point &np, const cv::Mat &mat)
{
	Helper help;
	return  help.inMat(np, mat.rows, mat.cols);
}
cv::Mat FindEyeCenter::floodKilledges(cv::Mat &mat)
{
	rectangle(mat, cv::Rect(0, 0, mat.cols, mat.rows), 255);
	cv::Mat mask(mat.rows, mat.cols, CV_8U, 255);
	queue<cv::Point> toDo;
	toDo.push(cv::Point(0, 0));
	while (!toDo.empty())
	{
		cv::Point p = toDo.front();
		toDo.pop();
		if (mat.at<float>(p) == 0.0f)
		{
			continue;
		}
		//-- agrgar las posiciones
		cv::Point np(p.x + 1, p.y); // derecha
		if (floodShouldpushPoint(np, mat))
		{
			toDo.push(np);
		}

		{

		}
	}
	return mask;
}








