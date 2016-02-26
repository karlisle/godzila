#include "captureFrame.h"
#include "hough.h"
using namespace std;

int main(int argc, char** argv)
{
	cout << "\t\n\nOk, ahora curso intensivo de vectores!" << endl;
	cout << "\n" << endl;

	//CaptureFrame begin;
	//begin.capture();	

	NewHough hough;
	hough.find_contours();

	//Detener la ejecucion
	cin.get();
	return 0;
}