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



	//	ShapeFun.showShapeFunctions();

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

		mesh.divCbyTimeStep(data.get_timeStep()*(iter + 1));

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

		//cout << *min_element(vectorTemperature, vectorTemperature + (data.get_numberOfNodes())) << endl;
		//cout << *max_element(vectorTemperature, vectorTemperature + (data.get_numberOfNodes())) << endl;
		vectorTMAX[iter + 1] = *max_element(vectorTemperature, vectorTemperature + (data.get_numberOfNodes()));
		vectorTMIN[iter + 1] = *min_element(vectorTemperature, vectorTemperature + (data.get_numberOfNodes()));
		mesh.set_temp(vectorTemperature);
	}

	/*mesh.set_globalVectorP();
	mesh.addCdivTimeStepmultTemp2P();
	for (int i = 0; i < data.get_numberOfNodes(); i++)
	gaussMatrix[i][data.get_numberOfNodes()] = mesh.globalVectorP[i];
	cout << "\nShow gauss matrix\n";
	for (int i = 0; i < data.get_numberOfNodes(); i++)
	{
	for (int j = 0; j < data.get_numberOfNodes()+1; j++)
	cout << gaussMatrix[i][j] << "\t";
	cout << endl;
	}
	gauss(data.get_numberOfNodes(), gaussMatrix, vectorTemperature);
	cout << "\nnew temp vector:\n";
	for (int i = 0; i < data.get_numberOfNodes(); i++)
	cout << vectorTemperature[i] << "\t";
	cout << endl;
	cout << *min_element(vectorTemperature, vectorTemperature + (data.get_numberOfNodes())) << endl;
	cout << *max_element(vectorTemperature, vectorTemperature + (data.get_numberOfNodes())) << endl;
	mesh.set_temp(vectorTemperature);
	mesh.set_globalVectorP();
	mesh.addCdivTimeStepmultTemp2P();
	for (int i = 0; i < data.get_numberOfNodes(); i++)
	gaussMatrix[i][data.get_numberOfNodes()] = mesh.globalVectorP[i];
	gauss(data.get_numberOfNodes(), gaussMatrix, vectorTemperature);
	cout << "\nnew temp vector:\n";
	for (int i = 0; i < data.get_numberOfNodes(); i++)
	cout << vectorTemperature[i] << "\t";
	cout << endl;
	cout << *min_element(vectorTemperature, vectorTemperature + (data.get_numberOfNodes())) << endl;
	cout << *max_element(vectorTemperature, vectorTemperature + (data.get_numberOfNodes())) << endl;
	for (int i = 0; i < data.get_numberOfNodes(); i++)
	gaussMatrix[i][data.get_numberOfNodes()] = mesh.globalVectorP[i];
	*/

	//for (int i = 0; i < iterations; i++)
	//{


	//}



	//tu by siê gaus przyda³ zwracaj¹cy vector temp
	//tu by siê setter temperatur przyda³

	//	cout << "first one:" << endl;
	//	cout << "id: " << mesh.nodes[0][0].get_id() << "\tx: " << mesh.nodes[0][0].get_x() << endl;
	//	cout << "last one: " << endl;
	//	cout << "id: " << mesh.nodes[int(data.get_num_of_nodes_H() - 1)][int(data.get_num_of_nodes_L() - 1)].get_id() << "\tx: " 
	//		<< mesh.nodes[int(data.get_num_of_nodes_H() - 1)][int(data.get_num_of_nodes_L() - 1)].get_x() <<
	//		"\ty: " << mesh.nodes[int(data.get_num_of_nodes_H() - 1)][int(data.get_num_of_nodes_L() - 1)].get_y() << endl;


	//	cout << "id: " << mesh.nodes[2][3].get_id() << "\tx: " << mesh.nodes[2][3].get_x() << "\ty:  " << mesh.nodes[2][3].get_y() << endl;
	//	cout << "id: " << mesh.nodes[5][2].get_id() << "\tx: " << mesh.nodes[5][2].get_x() << "\ty: " << mesh.nodes[5][2].get_y() << endl;

	//	cout << "\n\nwhole grid shown by nodes\n" << endl;
	//	mesh.showNodesinGrid();

	//	cout << "\nedges\n";
	//	mesh.showNodesEdges();

	//	cout << "\n\nwhole grid by elements id\n";
	//	mesh.showElementsInGrid();

	/*	cout << "\n\nshow 0 element\n";
	mesh.gridElmnts[0][0].showElement();
	cout << "\nshow 1st element\n";
	mesh.gridElmnts[1][0].showElement();
	cout << "\nshow 4th element\n";
	mesh.gridElmnts[4][0].showElement();
	cout << "\nshow 5th element:\n";
	mesh.gridElmnts[0][1].showElement();
	cout << "\nshow 9th elemeny: \n";
	mesh.gridElmnts[4][1].showElement();
	cout << "\nshow 10th element:\n";
	mesh.gridElmnts[0][2].showElement();
	cout << "\nshow 14th element: \n";
	mesh.gridElmnts[4][2].showElement();
	*/

	//Node node1(0, 0, 0, 0, 1), node2(1, 0.025, 0, 0, 1), node3(2, 0.025, 0.025, 0, 0), node4(3, 0, 0.025, 0, 0);
	//Elmnt elmnt(0, node1, node2, node3, node4, ShapeFun, data);
	//elmnt.showElement();
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//		cout << elmnt.matrixH[i][j];
	//	cout << endl;
	//}


	//	cout << "\nSETUJ SIE PROSZEEEE: ";
	//	cout << mesh.nodes[0][0].get_id()<<"\t"<< mesh.nodes[0][0].get_temperature() << "\t" << mesh.nodes[3][2].get_id() << "\t" << mesh.nodes[3][2].get_temperature()<<
	//		"\t"<<mesh.nodes[1][1].get_id()<<"\t"<< mesh.nodes[1][1].get_temperature();

	//	vectorTemperature = mesh.get_temp();
	//	cout << "\nget vector:\n" << vectorTemperature[3] << "\t" << vectorTemperature[8] << endl;
	//	vectorTemperature[0] = 8;
	//	vectorTemperature[14] = 0;
	//	cout << "\nupdate vector:\n" << vectorTemperature[0] << "\t" << vectorTemperature[14] << endl;
	//	mesh.set_temp(vectorTemperature);
	//	cout << "\nUPDATEUJ SIE PROSZEEEE: ";
	//	cout << mesh.nodes[0][0].get_temperature() << "\t" << mesh.nodes[3][2].get_temperature() << "\t" << mesh.nodes[1][1].get_temperature() << endl;
	cout << "\n1MAX: " << vectorTMAX[0] << "\t1MIN: " << vectorTMIN[0] << endl;

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