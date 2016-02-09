
#include <iostream>
#include "inicializar.h"

using namespace std;

int main()
{
	cout << "\n\n\t Bienbenido a Eye-Gaze Focus..." << endl;
	cout << "\n\n\n";
	cout << "Inicializando sistemas, espere....." << endl;

	// Instanciar la clase Inicializar
	Inicalizar comenzar;

	comenzar.detectFace();
	cin.get();
	return 0;
}