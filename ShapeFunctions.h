#pragma once
#include "IntegrationPoints.h"

class ShapeFunctions {
public:
	IntegrationPoints intPoints;

	double N[4][4];			// [shape function number][integration point number];
	double dNdKsi[4][4];	// derivative (d N / d ksi) [shape function number][integration point number]
	double dNdEta[4][4];	// derivative (d N / d eta) [shape function number][integration point number]

	ShapeFunctions(IntegrationPoints intPoints);
	ShapeFunctions();
	void showShapeFunctions();
};
