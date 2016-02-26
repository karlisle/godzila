
#include "captureFrame.h"

using namespace std;
using namespace cv;

void CaptureFrame::capture()
{
	NewHough hough;
	VideoCapture capture;
	capture.open(0);
	
	int key = 0;

	if (capture.isOpened())
	{
		Sleep(3000);
		while (true)
		{
			Mat frame;
			Mat clonFrame;
			capture.read(frame);
			clonFrame = frame.clone();

			if (clonFrame.rows == 0 || clonFrame.cols == 0)
			{
				cout << "ERROR: Ningun cuadro capturado!" << endl;
				break;
			}
			
			hough.hough_circle();

			if (waitKey(27) >= 0)
			{
				break;
			}
		}
	}
	else
	{
		cerr << "ERROR: Dispositivo no inicializado!" << endl;

	}
	cvDestroyAllWindows();
	capture.release();
	return;
}