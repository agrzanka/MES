#include <math.h>
#include"IntegrationPoints.h"

IntegrationPoints::IntegrationPoints()
{
	this->ksi[0] = -(1 / (sqrt(3)));
	this->ksi[1] = (1 / (sqrt(3)));
	this->ksi[2] = (1 / (sqrt(3)));
	this->ksi[3] = -(1 / (sqrt(3)));

	this->eta[0] = -(1 / (sqrt(3)));
	this->eta[1] = -(1 / (sqrt(3)));
	this->eta[2] = (1 / (sqrt(3)));
	this->eta[3] = (1 / (sqrt(3)));
}