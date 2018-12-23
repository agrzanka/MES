#pragma once
#include "IntegrationPoints.h"

class ShapeFunctions {
public:
	IntegrationPoints intPoints;
	/*double N1[4];
	double N2[4];
	double N3[4];
	double N4[4];*/
	double N[4][4];


	ShapeFunctions(IntegrationPoints intPoints);
	void showShapeFunctions();
};
