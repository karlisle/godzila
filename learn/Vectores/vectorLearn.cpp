#include "vectorLearn.h"

using namespace std;

void Vectores::comenzar()
{
	vector<vector<vector<int>>> qwerty(4, vector<vector<int>>(3, vector<int>(2, 0) ) );

	

	for (int i = 0; i < qwerty.size(); i++)
	{
		for (int j = 0; j < qwerty[0].size(); j++)
		{
			for (int k = 0; k < qwerty[0][0].size(); k++)
			{
				//cout << qwerty[i][j][k]<< endl;
				qwerty[i][j][k] = k;
			}
		}
	}
	for (int i = 0; i < qwerty.size(); i++)
	{
		for (int j = 0; j < qwerty[0].size(); j++)
		{
			for (int k = 0; k < qwerty[0][0].size(); k++)
			{
				cout << qwerty[i][j][k] << endl;
			}
		}
	}
}