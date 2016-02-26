
#include "newhoug.h"

bool myfunction(Point4i i, Point4i j) { return (j.val < i.val); }
bool myfunction2(TPoint6m i, TPoint6m j) { return (i.u < j.u); }

vector<Vec3i> NewHough::circle_hough(Mat edges, vector<int> range, bool same, bool normalise, int npeaks, Mat GX, Mat GY, Mat mag, int options)
{
	// Obtener las coordenadas de los ejes
	cout << "Hola desde New Hough!!" << endl;
	vector<Point> idx;
	vector<vector<Point3i>> lut;
	unsigned char *rowPtr;

	for (int i = 0; i < edges.rows; ++i)
	{
		rowPtr = edges.ptr<unsigned char>(i);
		for (int j = 0; j < edges.cols; ++j)
		{
			if (rowPtr[j] > 0)
			{
				//cout << "Ya pasamos de aqui!!" << endl;
				idx.push_back(Point2d(i, j));

			}
		}
	}
	// No retornar circulos si el borde no existe
	if (idx.size() < 1)
	{
		cout << "idx: Vacio!!" << endl;
		return vector<Vec3i>();
	}
	// si los bordes existen
	// Cuantizar el gradiente a lo largo de los bordes
	vector<int> qGrad(idx.size());
	for (int i = 0; i < idx.size(); ++i)
	{
		int dir;
		if (GY.at<float>(idx[i].x, idx[i].y) < 0 )
		{
			dir = (360 + atan2(GY.at<float>(idx[i].x, idx[i].y), GX.at<float>(idx[i].x, idx[i].y)) * 180 / PI) / 45.0;
		}
		else
		{
			dir = ( atan2 (GY.at<float>(idx[i].x, idx[i].y), GX.at<float>(idx[i].x, idx[i].y)) * 180 / PI) / 45.0;
		}
		qGrad[i] = dir;
	}
	// agrgar un margen que evite tener que revisar los enlaces
	int nr = edges.rows;
	int nc = edges.cols;
	int nradii = range.size();
	int margin = ceil(*(max_element(range.begin(), range.end()))) + 2;
	int nrh = nr + 2 * margin + 1;        // increase size of accumulator
	int nch = nc + 2 * margin + 1;
	hAccumulator = vector< vector< vector<double>>>(nrh, vector< vector< double>>(nch, vector<double>(nradii, 0)));
	double maxMag, minMag;
	minMaxLoc(mag, &minMag, &maxMag);

	// Calcular el acumulador de captura de acuerdo a las opciones pasadas.
	if (options & OPTIONS::NO_GRADIENT)
	{
		cout << "NO_GRADIENT" << endl;
		// No usar la direccion del gradiente del borde
		for (int i = 0; i < nradii; ++i)
		{
			// Iterar sobre las caracteristicas.
			for (int f = 0; f < idx.size(); ++f)
			{
				for (int j = 0; j < templates[range[i] - 1].size(); ++j)
				{
					Point2i pt = templates[range[i]][j];
					if (options & OPTIONS::EDGE_WEIGHTING)
					{
						if ((pt.x + idx[f].x) > 0 && (pt.y + idx[f].y) > 0 && (pt.x + idx[f].x) < edges.rows && (pt.y + idx[f].y) < edges.cols )
						{
							hAccumulator[ pt.x + idx[f].x + margin ][ pt.y + idx[f].y + margin ][i] += mag.at<float>(pt.x + idx[f].x, pt.y + idx[f].y) * 255 /maxMag;
						}
					}
				}
				
			}
		}
	}
	
	if (options & OPTIONS::GRADIENT)
	{
		cout << "GRADIENT" << endl;
		// Usar la direccion del gradiente del borde
		double gx, gy, rx, ry, prod;
		for (int i = 0; i < nradii; ++i)
		{
			// Iterar sobre las caracteristicas
			for (int  f = 0; f < idx.size(); ++f)
			{
				gx = GX.at<float>(idx[f].x, idx[f].y);
				gy = GY.at<float>(idx[f].x, idx[f].y);
				for (int j = 0; j < templates[range[i]].size(); ++j)
				{
					Point2d pt = templates[range[i]][j];
					// Obtener el gradiente del circulo actual
					rx = pt.x;
					ry = pt.y;
					prod = rx * gy + ry * gx;
					if (prod < 0)
					{
						if (options & OPTIONS::EDGE_WEIGHTING)
						{
							if ( (pt.x + idx[f].x) > 0 && (pt.y + idx[f].y) > 0 && (pt.x + idx[f].x) < edges.rows && (pt.y +idx[f].y) < edges.cols )
							{
								hAccumulator[pt.x + idx[f].x + margin][pt.y + idx[f].y + margin][i] += mag.at<float>(pt.x + idx[f].x, pt.y + idx[f].y) * 255 / maxMag;
							}
							else;
						}
						else
						{
							hAccumulator[pt.x + idx[f].x + margin][pt.y + idx[f].y + margin][i] += 1;
						}

					}
				}
			}
		}
	}
	
	
	if (options & OPTIONS::QUANTIZED_GRADIENT)
	{
		cout << "QUANTIZED_GRADIENT" << endl;
		for (int f = 0; f < idx.size(); ++f)
		{
			//cout << "Primer for" << endl;
			for (int r = 0; r < range.size(); ++r)
			{
				
				vector<Point3i>& points = lut[8 * (range[r] + qGrad[f])];
				cout <<"Points Size()" << points.size() << endl;
				for (int i = 0; i < points.size(); ++i)
				{
					//cout << "Tercer for" << endl;
					if (options & OPTIONS::EDGE_WEIGHTING)
					{
						if ((points[i].x + idx[f].x) > 0 && (points[i].y + idx[f].y) > 0 && (points[i].x + idx[f].x) < edges.rows && (points[i].y + idx[f].y) < edges.cols)
						{
							hAccumulator[points[i].x + idx[f].x + margin][points[i].y + idx[f].y + margin][r] += mag.at<float>(points[i].x + idx[f].x, points[i].y + idx[f].y) * 255 / maxMag;
						}
						else;
					}
					else
					{
						hAccumulator[points[i].x + idx[f].x + margin][points[i].y + idx[f].y + margin][r] += 1;
					}
				}
				
			}
		}
	}
	
	

	if (same)
	{
		cout << "Remover picos" << endl;
		// Remover los picos en el margen del area
		for (int  r = 0; r < nradii; ++r)
		{
			for (int i = 0; i < margin + 1; ++i)
			{
				for (int j = 0; j < hAccumulator[0].size(); ++j)
				{
					hAccumulator[i][j][r] = 0;
				}
			}
		}

		for (int r = 0; r < nradii; ++r)
		{
			for (int i = hAccumulator.size() - margin +1; i < hAccumulator.size(); ++i)
			{
				for (int j = 0; j < hAccumulator[0].size(); ++j)
				{
					hAccumulator[i][j][r] = 0;
				}
			}
		}

		for (int r = 0; r < nradii; ++r)
		{
			for (int i = 0; i < hAccumulator.size(); ++i)
			{
				for (int j = 0; j < margin + 1; ++j)
				{
					hAccumulator[i][j][r] = 0;
				}
			}
		}

		for (int r = 0; r < nradii; ++r)
		{
			for (int i = 0; i < hAccumulator.size(); ++i)
			{
				for (int j = hAccumulator[0].size() - margin; j < hAccumulator[0].size(); ++j)
				{
					hAccumulator[i][j][r] = 0;
				}
			}
		}

		if (normalise)
		{
			cout << "NORMALIZED" << endl;
			for (int r = 0; r < nradii; ++r)
			{
				for (int i = 0; i < hAccumulator.size(); ++i)
				{
					for (int j = 0; j < hAccumulator[0].size(); ++j)
					{
						hAccumulator[i][j][r] /= range[r];
					}
				}
			}

		}

	}

	//cout << "Llamando a circle_houghpeaks...." << endl;
	
	vector<Vec3i> peaks;
	vector<vector<vector<double>>> vectorRaro = hAccumulator;
	for (int i = 0; i < vectorRaro.size(); i++)
	{
		for (int j = 0; j < vectorRaro[0].size(); j++)
		{
			for (int k = 0; k < vectorRaro[0][0].size(); k++)
			{
				//cout << vectorRaro[i][j][k] << endl;
				peaks.push_back(Vec3i(i, j, k));

			}
		}
	}
	
	return peaks;
}

/*
vector<Vec3i> NewHough::circle_houghpeaks(vector<vector<vector<double>>> &H, vector<int> range, int npeaks, int margin)
{


	
	/*
	// Find at max 10 candidates
	vector<Vec3i> peaks;
	// calcular valor maximo
	double mv = 0;

	for (int i = 0; i < H.size(); ++i) 
	{
		for (int j = 0; j < H[0].size(); ++j)
		{
			for (int  k = 0; k < H[0][0].size(); ++k)
			{
				if (H[i][j][k] > mv)
				{
					mv = H[i][j][k];
					//cout << "mv = " << H[i][j][k] << endl;
				}
			}
		}
	}
	
	
	// Retener unicamente el 50% de los valores maximos
	// mv *= 0.5;

	// Obtener los maximos aceptados
	vector<Point4i> maxima;
	
	for (int k = 0; k < range.size(); ++k)
	{
		for (int j = 0; j < H[0].size(); ++j)
		{
			for (int i = 0; i < H.size(); ++k)
			{
				//cout << "mv = "<< mv << endl;
				//cout << "i = " << i << " J = "<< j << " k = " << k << endl;
				//cout << "H[k][j][i] = " << H[k][j][i] << endl;
				//maxima.push_back(Point4i(i, j, k, H[i][j][k]));
				if (H[i][j][k] > 0 && H[i][j][k])
				{
					maxima.push_back(Point4i(i, j, k, H[i][j][k]));
				}
				cout << "Algo raro pasa aqui....dentro!!" << endl;
			}
		}
	}
	//sort(maxima.begin(), maxima.end(), myfunction);
	
	for (int i = 0; i < maxima.size(); ++i)
	{
		if (maxima[i].val >= 0.8 * maxima[0].val && peaks.size() < npeaks)
		{
			int y = maxima[i].i - margin;
			int x = maxima[i].j - margin;
			peaks.push_back(Vec3i(maxima[i].i - margin, maxima[i].j - margin, range[maxima[i].k]));
		}
		else
		{
			cout << "ERROR: Algo paso con los picos!!" << endl;
			break;
		}
	}
	
	return;

}
*/



// circlepoints
vector<Point2i> NewHough::circlepoints(int r) {
	// Get number of rows needed to cover 1/8 of the circle
	int l = floor(r / sqrt(2) + 0.5);
	if (floor(sqrt(r * r - l * l) + 0.5) < l)   // % if crosses diagonal
		l = l - 1;

	// generate coords for 1/8 of the circle, a dot on each row
	vector<int> x0(l + 1);
	vector<int> y0(l + 1);
	for (int i = 0; i <= l; ++i) {
		x0[i] = i;
		y0[i] = floor(sqrt(r * r - x0[i] * x0[i]) + 0.5);
	}

	// Check for overlap
	int l2;
	if (y0[y0.size() - 1] == l)
		l2 = l;
	else
		l2 = l + 1;

	// Assemble first quadrant
	vector<int> x = x0;
	for (int i = l2 - 1; i > 0; --i)
		x.push_back(y0[i]);
	vector<int> y = y0;
	for (int i = l2 - 1; i > 0; --i)
		y.push_back(x0[i]);

	// Add next quadrant
	x0 = x;
	for (int i = 0; i < y.size(); ++i)
		x0.push_back(y[i]);
	y0 = y;
	for (int i = 0; i < x.size(); ++i)
		y0.push_back(-x[i]);

	// Assemble full circle
	x = x0;
	for (int i = 0; i < x0.size(); ++i)
		x.push_back(-x0[i]);
	y = y0;
	for (int i = 0; i < y0.size(); ++i)
		y.push_back(-y0[i]);

	vector<Point2i> points(x.size());
	for (int i = 0; i < x.size(); ++i)
		points[i] = Point2i(x[i], y[i]);
	//	points[i] = Point2i(y[i],x[i]);

	return points;
}

// getLUT
vector<vector<Point3i>> NewHough::getLUT(vector<int> range) {
	vector<Point3i> deg0, deg45, deg90, deg135, deg180, deg225, deg270, deg315;
	vector<vector<Point3i>> table(8 * range.size());
	for (int i = 0; i < range.size(); ++i) {
		deg0.clear();
		deg45.clear();
		deg90.clear();
		deg135.clear();
		deg180.clear();
		deg225.clear();
		deg270.clear();
		deg315.clear();
		// Get all template points
		vector<Point2i> points = circlepoints(range[i]);
		// Compute inner product between each point and each gradient direction and populate the point vectors
		for (int j = 0; j < points.size(); ++j) {
			float rx = points[j].x;
			float ry = points[j].y;

			double gx = 1;
			double gy = 0;

			double prod = rx*gy + ry*gx;
			if (prod < 0)
				deg0.push_back(Point3i(points[j].x, points[j].y, i));

			gx = 0.71;
			gy = 0.71;
			prod = rx*gy + ry*gx;
			if (prod < 0)
				deg45.push_back(Point3i(points[j].x, points[j].y, i));

			gx = 0;
			gy = 1;
			prod = rx*gy + ry*gx;
			if (prod < 0)
				deg90.push_back(Point3i(points[j].x, points[j].y, i));

			gx = -0.71;
			gy = 0.71;
			prod = rx*gy + ry*gx;
			if (prod < 0)
				deg135.push_back(Point3i(points[j].x, points[j].y, i));

			gx = -1;
			gy = 0;
			prod = rx*gy + ry*gx;
			if (prod < 0)
				deg180.push_back(Point3i(points[j].x, points[j].y, i));

			gx = -0.71;
			gy = -0.71;
			prod = rx*gy + ry*gx;
			if (prod < 0)
				deg225.push_back(Point3i(points[j].x, points[j].y, i));

			gx = 0;
			gy = -1;
			prod = rx*gy + ry*gx;
			if (prod < 0)
				deg270.push_back(Point3i(points[j].x, points[j].y, i));

			gx = 0.71;
			gy = -0.71;
			prod = rx*gy + ry*gx;
			if (prod < 0)
				deg315.push_back(Point3i(points[j].x, points[j].y, i));
		}

		table[8 * (range[i] - 1) + 0] = deg0;
		table[8 * (range[i] - 1) + 1] = deg45;
		table[8 * (range[i] - 1) + 2] = deg90;
		table[8 * (range[i] - 1) + 3] = deg135;
		table[8 * (range[i] - 1) + 4] = deg180;
		table[8 * (range[i] - 1) + 5] = deg225;
		table[8 * (range[i] - 1) + 6] = deg270;
		table[8 * (range[i] - 1) + 7] = deg315;
	}

	return table;
}