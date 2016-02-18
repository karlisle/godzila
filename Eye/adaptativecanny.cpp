
#include "adaptativecanny.h"

Mat AdaptativeCanny::adaptativeCanny(Mat img, Mat& GX, Mat& GY, Mat& mag, int& maxTH, int& minTH) 
{
	/*
	*	 Calcular el umbral adptativo
	*/
	// Magic numbers
	const double PercentOfPixelsNotEdges = 0.9;		// Usado para el umbral seleccionado
	const double ThresholdRatio = 0.3;				// 
	const double sigma = sqrt(2);					//

	// covertir a una doble matriz 
	Mat imgd(img.rows, img.cols, CV_64F);
	img.convertTo(imgd, CV_64F, 1.0 / 255, 0);

	//  Calcular el gradiente usando una derivada de un filtro Gaussiano
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_32F;
	Sobel(img, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	Sobel(img, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);

	Mat magGrad, pgx, pgy;
	pow(grad_x, 2, pgx);
	pow(grad_y, 2, pgy);
	magGrad = pgx + pgy;

	cv::sqrt(magGrad, magGrad);

	// Calcular la magnitud del gradiente
	GX = grad_x;
	GY = grad_y;
	mag = magGrad;
	// Normalizar el umbral de seleccion
	double minVal;
	double maxVal;
	Point minLoc;
	Point maxLoc;
	// No se hace nada 
	// Determinar el umbral de histéresis
	double lowThresh, highThresh;
	selectThreshold(magGrad, PercentOfPixelsNotEdges, ThresholdRatio, lowThresh, highThresh);
	// Invocar Canny normal
	Mat edges(img.rows, img.cols, CV_8U);
	// Copiar la matriz para no arruinar la original
	Mat imgCopy = img.clone();
	GaussianBlur(imgCopy, imgCopy, Size(15, 15), 2, 2); // Reducir el tamaño del filtro
	Canny(imgCopy, edges, 60, 128, 3, true);
	// Realizar No-Maximum Supression Thining y Hysterisis Thresholdinf a la intensidad del borde
	maxTH = 255 * highThresh;
	minTH = 255 * lowThresh;

	return edges;
}

void AdaptativeCanny::selectThreshold(cv::Mat magGrad, double PercentOfPixelsNotEdges, double ThresholdRatio, double& lowThresh, double& highThresh)
{
	int m = magGrad.rows;
	int n = magGrad.cols;
	// Selecion del umbral
	vector<int> chanels;
	chanels.push_back(0);
	MatND counts;
	vector<int> size;
	size.push_back(64);
	vector<float> range;
	range.push_back(0);
	range.push_back(255);
	vector<Mat> vec;
	vec.push_back(magGrad);
	calcHist(vec, chanels, Mat(), counts, size, range, true);
	// Calcular el umbral alto
	vector<int> csum(counts.rows);
	cout << csum[0] << endl;

	csum[0] = counts.at<float>(0);

	for (int i = 1; i < counts.rows; ++i)
	{
		csum[i] = counts.at<float>(i) + csum[i - 1];
	}
	for (int i = 1; i < counts.rows; ++i)
	{
		if (csum[i] > PercentOfPixelsNotEdges * m * n)
		{
			highThresh = i / 64.0;
			break;
		}
		lowThresh = ThresholdRatio * highThresh;
	}

}