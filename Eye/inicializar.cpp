#pragma once

#include "inicializar.h"

void drawPose(cv::Mat& img, const cv::Mat& rot, float lineL)
{
	int loc[2] = { 70, 70 };
	int thickness = 2;
	int lineType = 8;

	cv::Mat P = (cv::Mat_<float>(3, 4) <<
		0, lineL, 0, 0,
		0, 0, -lineL, 0,
		0, 0, 0, -lineL);
	P = rot.rowRange(0, 2)*P;
	P.row(0) += loc[0];
	P.row(1) += loc[1];
	cv::Point p0(P.at<float>(0, 0), P.at<float>(1, 0));

	line(img, p0, cv::Point(P.at<float>(0, 1), P.at<float>(1, 1)), cv::Scalar(255, 0, 0), thickness, lineType);
	line(img, p0, cv::Point(P.at<float>(0, 2), P.at<float>(1, 2)), cv::Scalar(0, 255, 0), thickness, lineType);
	line(img, p0, cv::Point(P.at<float>(0, 3), P.at<float>(1, 3)), cv::Scalar(0, 0, 255), thickness, lineType);
}

int Inicializar::detect()
{
	// Inicializar variables y la estructura de datos.
	int frameWidth = 1;
	int frameHeight = 1;
	int D = 1;
	int H = 1;
	NewHough::OPTIONS detectionMode = NewHough::QUANTIZED_GRADIENT;
	//Trainer::MODEL gazeModel;
	bool maxDensity = false;
	bool binaryThresholding = false;
	float k[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };
	const Mat K(3, 3, CV_32F, (char*)k);
	bool blink = false;
	bool acquire = false;
	EyeGaze leftGaze, rightGaze;

	string videoName;
	string outputName;

	
	// Cargar los modeos entrenados y  los binarios de IntraFace
	char detectionModel[] = "models/DetectionModel-v1.5.bin";
	char trackingModel[] = "models/TrackingModel-v1.10.bin";
	string faceDetectionModel("models/haarcascade_frontalface_alt2.xml");


	cout << "Iniciando componentes y variables del sistema Eye-gaze...!!" << endl;
	// Inicualizar objeto XXDescriptor
	INTRAFACE::XXDescriptor xxd(4);
	// Inicializar objeto de FaceAligment
	INTRAFACE::FaceAlignment fa(detectionModel, trackingModel, &xxd);

	if (!fa.Initialized())
	{
		cerr << "FaceAligment cannot be initialized" << endl;
		return -1;
	}
	// Cargar el modelo de detecion de rostros de OpenCV
	cv::CascadeClassifier face_cascade;
	if (!face_cascade.load(faceDetectionModel))
	{
		cerr << "Error: No se ha podido cargar el modelo de teccion de rostos!!" << endl;
		return -1;
	}

	// Inicializar variables para el loop principal
	// Inicializar dispositivo de captur.
	// Posteriormente se seleccionara a partir de un dialogo.

	VideoCapture capture;
	capture.open(0);
	Mat fOrig;
	Mat lEye;
	Mat rEye;
	Mat lEyeBW;
	Mat rEyeBW;
	Mat leftEyeBW;
	Mat rightEyeBW;
	Vec3i lCircle;
	Vec3i rCircle;
	// Creamos unas ventanas
	namedWindow("Gaze", CV_WINDOW_NORMAL);
	namedWindow("LeftEye", CV_WINDOW_AUTOSIZE);
	namedWindow("RightEye", CV_WINDOW_AUTOSIZE);
	int key = 0;
	bool isDetect = true;
	float score, notFace = 0.3;
	Mat X, X0;
	vector<Rect> faces;
	vector<Rect> eyes;
	// Si el dispositivo se inicializo correctamente se inicia el ciclo principal.
	if (capture.isOpened())
	{
		// El siguinete delay es para dar tiempo a que el dispositivo 
		// termine de cargar
		Sleep(2000);
		while (true)
		{
			Mat frame;
			Mat frameOrig;
			capture.read(frameOrig);					// Se captura un frame
			frame = frameOrig.clone();
			fOrig = frame.clone();
			// Se comprueba que el frame contenga informacion
			// en caso contrario terminamos
			if (frame.rows == 0 || frame.cols == 0)
			{
				cout << "Error: No frame!!" << endl;
				break;
			}
			// Lo siguiente se ejecuta solo si isDetect es igual a true.
			// En otras palabras, solo si se ha detectado un rostro.
			if (isDetect)
			{
				// Detectamos los rostros del frame
				face_cascade.detectMultiScale(frame, faces, 1.2, 2, 0, Size(40, 40));
				// Comprobamos si el vector de rostros esta vacio
				// En caso de estarlo se muestra el frameactual 
				// y se contia a la siguiente instruccion
				if (faces.empty())
				{
					//imshow("Main", frame);
					key = waitKey(5);
					continue;
				}
				// Detectar las caracteristicas en el rostro mas grande detectado
				// En este punto se envia el frame contenido en el vector <faces>
				// con mayores dimenciones (HxW), en caso de no ser un rostro, se termina.
				if (fa.Detect(frame, faces[0], X0, score) != INTRAFACE::IF_OK)
				{
					break;
				}
				// Si el vector <faces> esta vacio y/o se detecta que no es un rostro, 
				// isDetect se pone a "false"!
				isDetect = false;
			}
			else
			{
				// Seguimiento de las marcas faciales en el fotograma de entrada
				// En caso de no haberla, se termina le ejecucion
				if (fa.Track(frame, X0, X, score) != INTRAFACE::IF_OK)
				{
					break;
				}
				// Asignamos las nuevas marcas faciales a X0.
				// Donde X0 = actuales y X = anteriores
				X0 = X;
			}
			if (score < notFace)
			{
				isDetect = true;
			}
			else
			{
				// Dibujamos los marcadores faciales
				for (int i = 0; i < faces.size(); i++)
				{
					circle(frame, Point((int)X0.at<float>(0, i), (int)X0.at<float>(1, i)), 2, Scalar(0, 255, 0), -1);
				}
				// Estimar la posicion de la cabeza
				INTRAFACE::HeadPose hp;
				fa.EstimateHeadPose(X0, hp);
				// Dibujamos los ejes de la posicion.
				drawPose(frame, hp.rot, 50);
				cout << ".";

				// Posicion estimada de la cabeza [mm]
				circle(frame, Point( (int)X0.at<float>(0,28), (int)X0.at<float>(1,28)), 2, Scalar(0, 0, 255), -1);
				circle(frame, Point((int)X0.at<float>(0, 19), (int)X0.at<float>(1, 19)), 2, Scalar(255, 0, 0), -1);
				Point El(X0.at<float>(0, 28), X0.at<float>(1, 28));
				Point Er(X0.at<float>(0, 19), X0.at<float>(1, 19));
				Point3i pos = leftGaze.computeHeadposition(hp.rot, K, El, Er, H, D);
				// Imprimir rotacion de la cabeza
				hp.rot.at<float>(0, 0);
				// Mostrar la posicion de la caveza
				putText( frame, "Head position [mm]: x = " + to_string(pos.x) + " Y = " + to_string(pos.y) + " z = " + to_string(pos.z),
					Point(200, 70), FONT_HERSHEY_SIMPLEX, 0.3, 255 );
				
				// Identificar los ojos
				// Puntos del ojo izquierdo
				vector<Point> leftEyePoints;
				leftEyePoints.push_back(Point(X0.at<float>(0, 19), X0.at<float>(1, 19)));
				leftEyePoints.push_back(Point(X0.at<float>(0, 20), X0.at<float>(1, 20)));
				leftEyePoints.push_back(Point(X0.at<float>(0, 21), X0.at<float>(1, 21)));
				leftEyePoints.push_back(Point(X0.at<float>(0, 22), X0.at<float>(1, 22)));
				leftEyePoints.push_back(Point(X0.at<float>(0, 23), X0.at<float>(1, 23)));
				leftEyePoints.push_back(Point(X0.at<float>(0, 24), X0.at<float>(1, 24)));
				// Puntos del ojo derecho
				vector<Point> rightEyePoints;
				rightEyePoints.push_back(Point(X0.at<float>(0, 25), X0.at<float>(1, 25)));
				rightEyePoints.push_back(Point(X0.at<float>(0, 26), X0.at<float>(1, 26)));
				rightEyePoints.push_back(Point(X0.at<float>(0, 27), X0.at<float>(1, 27)));
				rightEyePoints.push_back(Point(X0.at<float>(0, 28), X0.at<float>(1, 28)));
				rightEyePoints.push_back(Point(X0.at<float>(0, 29), X0.at<float>(1, 29)));
				rightEyePoints.push_back(Point(X0.at<float>(0, 30), X0.at<float>(1, 30)));

				Rect leftBoundRect = boundingRect(leftEyePoints);
				Rect rightBoundRect = boundingRect(rightEyePoints);

				leftBoundRect.x = leftBoundRect.x - 5;
				leftBoundRect.y = leftBoundRect.y - 5;
				leftBoundRect.width = leftBoundRect.width + 10;
				leftBoundRect.height = leftBoundRect.height + 10;

				rightBoundRect.x = rightBoundRect.x - 5;
				rightBoundRect.y = rightBoundRect.y - 5;
				rightBoundRect.width = rightBoundRect.width + 10;
				rightBoundRect.height = rightBoundRect.height + 10;

				Mat leftEye = frameOrig(leftBoundRect);
				Mat rightEye = frameOrig(rightBoundRect);

				lEye = leftEye.clone();
				rEye = rightEye.clone();

				
				// Estimar el centro de los ojos
				try
				{
					// Extraer el ojo izquierdo
					lCircle = leftGaze.computeIrisCenter(leftEye, K, pos.z, binaryThresholding, maxDensity, 0, 0, detectionMode);
					circle(frame, Point(lCircle[1] + leftBoundRect.x, lCircle[0] + leftBoundRect.y), lCircle[0] /4, Scalar(0, 0, 255));
					circle(frame, Point(lCircle[1] + leftBoundRect.x, lCircle[0] + leftBoundRect.y), 1, Scalar(0, 0, 255));
					lEyeBW = leftGaze.getEqualizedEye();
					cvtColor(lEyeBW, leftEyeBW, CV_GRAY2BGR);
					
					// Extraer el ojo derecho
					rCircle = rightGaze.computeIrisCenter(rightEye, K, pos.z, binaryThresholding, maxDensity, 0, 0, detectionMode);
					circle(frame, Point(rCircle[1] + rightBoundRect.x, rCircle[0] + rightBoundRect.y), rCircle[0]/4, Scalar(0, 0, 255));
					circle(frame, Point(rCircle[1] + rightBoundRect.x, rCircle[0] + rightBoundRect.y), 1, Scalar(0, 0, 255));
					rEyeBW = rightGaze.getEqualizedEye();
					cvtColor(rEyeBW, rightEyeBW, CV_GRAY2BGR);
					
					Mat tmp;
					// Ojo Izquierdo
					cout << lCircle[0] << " : " << lCircle[1] << " : " << lCircle[2] << endl;
					circle(leftEyeBW, Point(lCircle[1], lCircle[0]), lCircle[1], Scalar(0, 0, 255));
					circle(leftEyeBW, Point(lCircle[1], lCircle[0]), 1, Scalar(0, 0, 255));
					resize(leftEyeBW, tmp, Size(3 * leftEye.cols, 3 * leftEye.rows));
					leftEyeBW = tmp.clone();
					// Ojo Derecho
					circle( rightEyeBW, Point(rCircle[1], rCircle[0]), rCircle[1], Scalar(0, 0, 255));
					circle( rightEyeBW, Point(rCircle[1], rCircle[0]), 1, Scalar(0, 0, 255));
					resize(rightEyeBW, tmp, Size(3*rightEye.cols, 3*rightEye.rows));
					rightEyeBW = tmp.clone();

					moveWindow("LeftEye", 110, 110);
					moveWindow("RightEye", 110 + 4 * leftBoundRect.width, 110);

					
					// Dibujar estimacion del iris
				}
				catch (IrisDetectionException e){
				}

				imshow("Gaze", frame);
				imshow("LeftEye", leftEyeBW);
				imshow("RightEye", rightEyeBW);
			
			}
			
			//imshow("Main", frame);

			if (waitKey(27) >= 0)
			{
				break;
			}

		}
	}
	cvDestroyAllWindows();
	capture.release();
	return 0;
// Fin del metodo
}
