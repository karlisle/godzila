
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

