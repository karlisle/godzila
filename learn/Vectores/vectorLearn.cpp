#include "vectorLearn.h"

using namespace std;

void Vectores::comenzar()
{
	string vector[4][4] = { {"1", "2", "3", "4"}, {"H", "O", "L", "A" } };	
	for (int i = 0; i <= 3; i++)
	{
		for (int j = 0; j <= 3; j++)
		{
			cout << vector[i][j] << endl;
		}
	}
}