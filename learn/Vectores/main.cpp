#include "captureFrame.h"

using namespace std;

int main(int argc, char** argv)
{
	cout << "\t\n\nOk, ahora curso intensivo de vectores!" << endl;
	cout << "\n" << endl;

	CaptureFrame begin;
	begin.detect();

	//Detener la ejecucion
	cin.get();
	return 0;
}