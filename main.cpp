#include <iostream>
#include "Input_data.h"
#include "Node.h"
#include "Grid.h"
#include "Elmnt.h"
#include "IntegrationPoints.h"
#include "ShapeFunctions.h"
#include<algorithm>

using namespace std;

double* gauss(int n, double ** AB, double *X);

int main()
{
	Input_data data;

	double t = data.get_tot();

	IntegrationPoints intPoints;
	ShapeFunctions ShapeFun(intPoints);

	double *vectorTemperature = new double[data.get_numberOfNodes()];
	int iterations = data.get_time() / data.get_timeStep();
	cout << "\n\niterations: " << iterations << endl;

	double** gaussMatrix = new double*[data.get_numberOfNodes()];
	for (int i = 0; i < data.get_numberOfNodes(); i++)
		gaussMatrix[i] = new double[(data.get_numberOfNodes() + 1)];

	double*vectorTMAX = new double[iterations + 1];
	double*vectorTMIN = new double[iterations + 1];


	Grid mesh(data, ShapeFun);

	vectorTemperature = mesh.get_temp();

	vectorTMAX[0] = *max_element(vectorTemperature, vectorTemperature + (data.get_numberOfNodes()));
	vectorTMIN[0] = *min_element(vectorTemperature, vectorTemperature + (data.get_numberOfNodes()));


	for (int iter = 0; iter < iterations; iter++)
	{
		mesh.set_globalMatrixH();
		mesh.show_globalMatrixH();
		mesh.set_globalMatrixC();
		mesh.show_globalMatrixC();

		mesh.divCbyTimeStep(data.get_timeStep());

		mesh.addCdivTimeStep2H();

		for (int i = 0; i < data.get_numberOfNodes(); i++)
			for (int j = 0; j < data.get_numberOfNodes(); j++)
				gaussMatrix[i][j] = mesh.globalMatrixH[i][j];

		cout << "\nShow gauss matrix\n";
		for (int i = 0; i < data.get_numberOfNodes(); i++)
		{
			for (int j = 0; j < data.get_numberOfNodes() + 1; j++)
				cout << gaussMatrix[i][j] << "\t";
			cout << endl;
		}

		mesh.set_globalVectorP();
		mesh.addCdivTimeStepmultTemp2P();

		for (int i = 0; i < data.get_numberOfNodes(); i++)
			gaussMatrix[i][data.get_numberOfNodes()] = mesh.globalVectorP[i];

		vectorTemperature = gauss(data.get_numberOfNodes(), gaussMatrix, vectorTemperature);

		vectorTMAX[iter + 1] = *max_element(vectorTemperature, vectorTemperature + (data.get_numberOfNodes()));
		vectorTMIN[iter + 1] = *min_element(vectorTemperature, vectorTemperature + (data.get_numberOfNodes()));
		mesh.set_temp(vectorTemperature);
	}



	cout << "\n\n\n\t\t\tRESULT:\n\n";
	for (int i = 0; i < iterations + 1; i++)
		cout << "iteration number: " << i << "\tTemp min: " << vectorTMIN[i] << "\tTemp max: " << vectorTMAX[i] << endl;



	system("PAUSE");
}

double* gauss(int n, double ** AB, double *X) {
	int i, j, k;
	double m, s;

	for (i = 0; i < n - 1; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			if (fabs(AB[i][i]) < 1e-11)
				return NULL;
			m = -(AB[j][i]) / AB[i][i];
			for (k = i + 1; k <= n; k++)
				AB[j][k] += m * AB[i][k];
		}
	}

	for (i = n - 1; i >= 0; i--)
	{
		s = AB[i][n];
		for (j = n - 1; j >= i + 1; j--)
			s -= AB[i][j] * X[j];
		if (fabs(AB[i][i]) < 1e-11) return NULL;
		X[i] = s / AB[i][i];
	}
	return X;
}