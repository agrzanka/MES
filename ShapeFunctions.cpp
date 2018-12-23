#include "ShapeFunctions.h"
#include <iostream>

using namespace std;

ShapeFunctions::ShapeFunctions(IntegrationPoints intPnts) //for four-nodes element only!!!
{
	this->intPoints = intPnts;

	for (int index = 0; index < 4; index++)
	{
		this->N[0][index] = 0.25*(1 - intPnts.ksi[index])*(1 - intPnts.eta[index]);
		this->N[1][index] = 0.25*(1 + intPnts.ksi[index])*(1 - intPnts.eta[index]);
		this->N[2][index] = 0.25*(1 + intPnts.ksi[index])*(1 + intPnts.eta[index]);
		this->N[3][index] = 0.25*(1 - intPnts.ksi[index])*(1 + intPnts.eta[index]);
	}
}

void ShapeFunctions::showShapeFunctions()
{
	for (int index = 0; index < 4; index++)
	{
		cout << "\nN" << index + 1 << ":\n";
		cout << N[index][0] << "\t" << N[index][1] << "\t" << N[index][2] << "\t" << N[index][3] << endl;
	}
}
