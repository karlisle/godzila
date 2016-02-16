
#include <iostream>
#include "inicializar.h"
//#include "eyegaze.h"


using namespace std;

int main()
{
	cout << "\n\n\t Bienvenido a Eye-Gaze Focus..." << endl;
	cout << "\n\n\n";
	cout << "Inicializando sistemas, espere....." << endl;

	Inicializar inicio;
	inicio.detect();
	
	cin.get();
	cout << "Oprima 'Enter' para salir!!" << endl;
	return 0;
}