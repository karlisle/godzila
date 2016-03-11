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
}

cv::Point FindEyeCenter::eyeCenter(cv::Mat face, cv::Rect eye, std::string debugWindow)
{
	/* TODO: Code
	* Hacemos cosas locas con la imagen para facilitar el procesamiento
	*/
	//
	//cout << "Encontrar el centro del ojo" << endl;

	cv::Mat eyeROIUnscaled = face(eye);
	cv::Mat eyeROI;
	scaleToFastSize(eyeROIUnscaled, eyeROI);
	//-- Creamos una ventana
	cv::namedWindow(debugWindow, CV_WINDOW_NORMAL);
	// dibujar la region del ojo
	rectangle(face, eye, 1234);
	// Encontrar el gradiente
	cv::Mat gradientX = computeMatXGradient(eyeROI);
	cv::Mat gradientY = computeMatXGradient(eyeROI.t()).t();
	// Normalizar y umbralizar el gradiente
	// Calcular rodas las magnitudes
	Helper help;
	cv::Mat mags = help.matrixMagnitude(gradientX, gradientY);
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

	//-- Create a blurred and inverted image for weighting 
	cv::Mat weight;
	GaussianBlur(eyeROI, weight, cv::Size(kWeightBlurSize, kWeightBlurSize), 0, 0);
	for (int y = 0; y < weight.rows; ++y)
	{
		unsigned char *row = weight.ptr<unsigned char>(y);
		for (int x = 0; x < weight.cols; ++x)
		{
			row[x] = (255 - row[x]);
		}
	}
	
	cv::imshow(debugWindow, weight);

	cv::Mat  outSum = cv::Mat::zeros(eyeROI.rows, eyeROI.cols, CV_64F);
	
	//printf("Eye Size: %ix%i\n", outSum.cols, outSum.rows);
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
		//cv::imshow(debugWindow + "Mask", mask);
		//cv::namedWindow("Salida", CV_WINDOW_NORMAL);
		//cv::imshow("Salida", out);
		cv::minMaxLoc(out, NULL, &maxVal, NULL, &maxP, mask);
	}
	return unscalePoint(maxP, eye);


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
		//-- derecha --
		cv::Point np(p.x + 1, p.y); 
		if (floodShouldpushPoint(np, mat))
		{
			toDo.push(np);
		}
		//-- Left  --
		np.x = p.x - 1;
		np.y = p.y;
		if (floodShouldpushPoint(np, mat))
		{
			toDo.push(np);
		}
		//-- Down --
		np.x = p.x;
		np.y = p.y + 1;
		if (floodShouldpushPoint(np, mat))
		{
			toDo.push(np);
		}
		//-- Up --
		np.x = p.x;
		np.y = p.y - 1;
		if (floodShouldpushPoint(np, mat))
		{
			toDo.push(np);
		}
		//-- Terminar --
		mat.at<float>(p) = 0.0f;
		mask.at<uchar>(p) = 0;
	}
	return mask;
}







