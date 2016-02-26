
#include "eyegaze.h"



Vec3i EyeGaze::computeIrisCenter(Mat eye, Mat K, float depth, bool thresh, bool maxD, int sColor, int sSpace, int options)
{
	// revisar el tipo de imagen y el tamaño
	if (eye.type() != CV_8UC3)
	{
		throw ImageTypeExeption();
		cout << "ERROR: Tipo incorrecto de imagen!!" << endl;
	}
	if (eye.rows < 1 || eye.cols < 1)
	{
		throw ImageSizeException();
		cout << "ERROR: El frame no contine informacion!!" << endl;
	}

	cout << "SUCCES:Procediendo!!" << endl;

	// Intentar ajustar la nitides de la imagen
	GaussianBlur(eye, eqEye, Size(0, 0), 10);
	addWeighted(eye, 1.5, eqEye, -0.5, 0, eye);

	// Si tamaño y el tipo de la imagen son correctos: pre-asignar matrices
	eqEye = eye.clone();
	Mat eyeTH = Mat(eqEye.rows, eqEye.cols, CV_8UC1);
	edges = Mat(eqEye.rows, eqEye.cols, CV_8UC1);
	Mat GX, GY, mag;
	int maxTH, minTH;

	// Filtrar la imagen RGB is es requerido
	if (sColor > 0 && sSpace > 0)
	{
		cout << "Filtrando rgb!!" << endl;
		Mat tmp = eqEye.clone();
		bilateralFilter(tmp, eqEye, -1, sColor, sSpace);
	}

	// Hacer borrosa la imagen para reducir el ruido
	//GaussianBlur(eye, eye, Size(3, 3), 0, 0, BORDER_DEFAULT);
	// Convertir la imagen filtrada o no, a escala de grises y estraer los canales de luminosidad 
	// de la imagen YUV  convertida
	cvtColor(eqEye, eqEye, CV_BGR2GRAY);
	// Maximizar el contraste de la imagen en grises
	equalizeHist(eqEye, eqEye);
	
	// Umbralizar la imagen con el metodo Otsu's, si es requerido (umbralizacion adaptativa)
	if (thresh)
	{
		cout << "Umbralizar la imagen" << endl;
		threshold(eqEye, eyeTH, 0, 255, CV_THRESH_BINARY_INV + THRESH_OTSU);
		// LLenar los huecos
		vector<vector<Point>> contours;
		findContours(eyeTH, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		if (contours.size() > 0)
		{
			for (int i = 0; i < contours.size(); i++)
			{
				drawContours(eyeTH, contours, i, 255, CV_FILLED);
			}
		}
	}

	// Calcular los bordes en la imagen en escala de grises umbralizada
	if (thresh)
	{
		// Aplicar un umbralizado adaptivo inteligente a la imagen umbralizada
		// En este caso no se usa
		edges = AdaptativeCanny::adaptativeCanny(eyeTH, GX, GY, mag, maxTH, minTH);
	}
	else
	{
		edges = AdaptativeCanny::adaptativeCanny(eqEye, GX, GY, mag, maxTH, minTH);
	}
	
	// Calcular el rango del Iris en [pxl] de acuerdo a la profundidad del ojo ([10, 14]) mm, o ([5,7]) mm de radio, en metros.
	// Para una imagen de 1280x720 la formula es: r = R * sqrt(fx^2 + fy^2) / D 
	// con R = radio [m], D = profundidad (m) y fx y fy = longitud focal [pxl], r = radio.

	// Encontrar el ajuste preciso del circulo con el diamtro del Iris
	//cout << K.size() << endl;
	//cout << "K at (0,0) = " << K << endl;
	
	//cout << "K at (0,0) = " << K.at<float>(0, 0) << endl;
	float  multiplier = sqrt(K.at<float>(0, 0) * K.at<float>(0, 0) + K.at<float>(1, 1) * K.at<float>(1, 1));
	//cout << "Multiplier = " << multiplier << endl;

	int minRadPxl = 0.8 * MIN_IRIS_RADIUS * multiplier / depth;
	//cout << "MinRad = " << minRadPxl << endl;

	int maxRadPxl = MAX_IRIS_RADIUS * multiplier / depth;
	//cout << "MaxRad = " << maxRadPxl << endl;

	range = vector<int>(min(10, maxRadPxl - minRadPxl + 1));
	//for (int i = 0; i < range.size(); ++i)
	//{
	//	cout << "range["<< i <<"] = " << range[i] << endl;
	//}

	int step = 1;

	if (range.size() == 10)
	{

		step = floor((maxRadPxl + minRadPxl + 1) / 10);
	}
	for (int i = 0; i < range.size(); ++i)
	{
		range[i] = minRadPxl + i * step;
	}

	// A
	Vec3i circle;
	//Sleep(2000);
	//vector<Vec3i> circles = hough.circle_hough(edges, range, true, true, 100, GX, GY, mag, options);
	//vector<int> bkjkd(6, 2);
	//vector<vector<Point3i>> valut = hough.getLUT(K);
	//cout << "Tamaño de valut =" << valut.size() << endl;
	//cout << valut[0][0].x << endl;
	/*
	if (circles.size() > 0)
	{
		if (maxD)
		{
			Mat intImage;
			vector<unsigned char> white(eqEye.rows * eqEye.cols, 255);
			Mat cplImage = Mat(eqEye.rows, eqEye.cols, CV_8U, white.data()) - eqEye;
			integral(cplImage, intImage);
			float maxDens;
			int idx = 0;
			for (int k = 0; k < circles.size(); ++k)
			{
				int minX = floor(max<float>(1, circles[k][1] - circles[k][2]));
				int minY = floor(max<float>(1, circles[k][0] - circles[k][2]));
				int maxX = floor(min<float>(eqEye.cols-1, circles[k][1] + circles[k][2]));
				int maxY = floor(min<float>(eqEye.rows - 1, circles[k][0] + circles[k][2]));
				float D = intImage.at<int>(maxY, maxX) + intImage.at<int>(minY, minX) - intImage.at<int>(minY, maxX) - intImage.at<int>(maxY, minX);

				D = D / ((maxX - minX) * (maxY - minY )* 255);
				if (D > maxDens)
				{
					maxDens = D;
					idx = k;
				}
			}
			circle = circles[idx];
		}
		else
		{
			circle = circles[0];
		}
	}
	return Vec3i(circle[0], circle[1], circle[2]);
	*/
}

Point3i EyeGaze::computeHeadposition(Mat R, Mat K, Point El, Point Er, int H, int D)
{
	int ul = El.x;
	int vl = El.y;
	int ur = Er.x;
	int vr = Er.y;
	double fx = K.at<float>(0, 0);
	double fy = K.at<float>(1, 1);
	int cx = K.at<float>(0, 2);
	int cy = K.at<float>(1, 2);

	//R = R';

	Vec3f r1, r2, r3;
	r1[0] = R.at<float>(0, 0);
	r1[1] = R.at<float>(0, 1);
	r1[2] = R.at<float>(0, 2);
	r2[0] = R.at<float>(1, 0);
	r2[1] = R.at<float>(1, 1);
	r2[2] = R.at<float>(1, 2);
	r3[0] = R.at<float>(2, 0);
	r3[1] = R.at<float>(2, 1);
	r3[2] = R.at<float>(2, 2);

	Point3f Olh(-D / 2.0, H, 0);
	Point3f Orh(D / 2.0, H, 0);

	double a = ur - cx;
	double b = ul - cx;
	double c = vr - cy;
	double d = vl - cy;

	double e = r3[0] * (Orh.x - Olh.x) + r3[1] * (Orh.y - Olh.y) + r3[2] * (Orh.z - Olh.z);

	double A = a * a * fy * fy + c * c * fx * fx + fx * fx * fy * fy;
	double B = b * b * fy * fy + d * d * fx * fx + fx * fx * fy * fy;
	double C = -2 * a * b * fy * fy - 2 * c * d * fx * fx - 2 * fx * fx * fy * fy;
	double E = -D * D * fx * fx * fy * fy;

	double z1 = (-(2 * A + C)* e + sqrt((2 * A + C) * (2 * A + C) * e * e - 4 * (A + B + C) * E)) / (2 * (A + B + C));
	double z2 = (-(2 * A + C)* e - sqrt((2 * A + C) * (2 * A + C) * e * e - 4 * (A + B + C) * E)) / (2 * (A + B + C));

	// Select the correct solution (z > 0)
	double zl = 0;
	if (z1 > 0)
		zl = z1;
	else
		zl = z2;

	// Sobsitute solution to find the remaining coordinates
	double tx = floor(zl * (ul - cx) / fx - (r1[0] * Olh.x + r1[1] * Olh.y + r1[2] * Olh.z) + 0.5);
	double ty = floor(zl * (vl - cy) / fy - (r2[0] * Olh.x + r2[1] * Olh.y + r2[2] * Olh.z) + 0.5);
	double tz = floor(zl - (r3[0] * Olh.x + r3[1] * Olh.y + r3[2] * Olh.z) + 0.5);

	return Point3i((int)tx, (int)ty, (int)tz);
}

