#include "Elmnt.h"
#include <math.h>

using namespace std;

Elmnt::Elmnt(int id, Node n1, Node n2, Node n3, Node n4, ShapeFunctions shapeFun)
{
	//this->set_edgeOfGrid(edge);
	this->set_id(id);
	this->set_nodes(n1, n2, n3, n4);
	this->set_shapeFunctions(shapeFun);
	this->set_interpolationOfCoordinates();
	this->set_transformationJacobian();
	this->set_detJ();
	this->set_revJacDivDetJ();
	this->set_dNdX();
	this->set_dNdY();
}

Elmnt::Elmnt()
{
	//this->set_interpolationOfCoordinates();
}

Elmnt::~Elmnt()
{

}


void Elmnt::set_id(int id)
{
	this->id = id;
}
void Elmnt::set_nodes(Node n1, Node n2, Node n3, Node n4)
{
	this->nodeID[0] = n1;
	this->nodeID[1] = n2;
	this->nodeID[2] = n3;
	this->nodeID[3] = n4;
}
/*void Elmnt::set_edgeOfGrid(bool edge)
{
this->edgeOfGrid = edge;
}*/


int Elmnt::get_id()
{
	return id;
}
/*bool Elmnt::is_edgeOfGrid()
{
return edgeOfGrid;
}*/



void Elmnt::set_shapeFunctions(ShapeFunctions shapeFun)
{
	this->shapeFun = shapeFun;
}

void Elmnt::set_interpolationOfCoordinates()
{
	cout << "\nInterpolation of coordinates in " << this->get_id() << " element:";
	for (int ip = 0; ip < 4; ip++)
	{
		xp[ip] = 0, yp[ip] = 0;
		for (int i = 0; i < 4; i++)
		{
			xp[ip] += shapeFun.N[i][ip] * nodeID[i].get_x();
			yp[ip] += shapeFun.N[i][ip] * nodeID[i].get_y();
		}
		cout << "\nxp " << ip << ": " << xp[ip] << "\t\typ " << ip << ": " << yp[ip];
	}
	cout << endl;
}


void Elmnt::set_transformationJacobian()
{
	for (int i = 0; i < 4; i++)
	{
		double tmpJxKSI = 0;	//dx/dksi
		double tmpJxETA = 0;	//dx/deta
		double tmpJyKSI = 0;	//dy/dksi
		double tmpJyETA = 0;	//dy/deta
		for (int index = 0; index < 4; index++)
		{
			tmpJxKSI += shapeFun.dNdKsi[index][i] * nodeID[index].get_x();
			tmpJxETA += shapeFun.dNdEta[index][i] * nodeID[index].get_x();
			tmpJyKSI += shapeFun.dNdKsi[index][i] * nodeID[index].get_y();
			tmpJyETA += shapeFun.dNdEta[index][i] * nodeID[index].get_y();
		}
		this->transfJacobian[i][0][0] = tmpJxKSI;
		this->transfJacobian[i][0][1] = tmpJyKSI;
		this->transfJacobian[i][1][0] = tmpJxETA;
		this->transfJacobian[i][1][1] = tmpJyETA;


	}
	cout << "\nTRANSFORMATION with JACOBIAN";
	for (int ip = 0; ip < 4; ip++)
	{
		cout << "\n" << ip << " integration point";
		cout << "\n1-1:\t" << transfJacobian[ip][0][0];
		cout << "\n1-2:\t" << transfJacobian[ip][0][1];
		cout << "\n2-1:\t" << transfJacobian[ip][1][0];
		cout << "\n2-2:\t" << transfJacobian[ip][1][1];
	}
}
void Elmnt::set_detJ()
{
	for (int i = 0; i < 4; i++)
	{
		this->detJ[i] = ((transfJacobian[i][0][0] * transfJacobian[i][1][1]) - (transfJacobian[i][0][1] * transfJacobian[i][1][0]));
	}
	cout << "\ndetJ 1: " << detJ[0] << "\tdetJ 2: " << detJ[1] << "\tdetJ 3: " << detJ[2] << "\tdetJ 4: " << detJ[3] << endl;
}

void Elmnt::set_revJacDivDetJ()
{
	cout << "\n\nreverse Jacobian divided by determinant f Jacobi matrix\n";
	for (int index = 0; index < 4; index++)
	{
		revJacDivDetJ[index][0][0] = (transfJacobian[index][1][1] / detJ[index]);
		revJacDivDetJ[index][0][1] = -(transfJacobian[index][0][1] / detJ[index]);
		revJacDivDetJ[index][1][0] = -(transfJacobian[index][1][0] / detJ[index]);
		revJacDivDetJ[index][1][1] = (transfJacobian[index][0][0] / detJ[index]);

		cout << index << " integration point: \n" << revJacDivDetJ[index][0][0] << "\t" << revJacDivDetJ[index][0][1];
		cout << "\t" << revJacDivDetJ[index][1][0] << "\t" << revJacDivDetJ[index][1][1] << endl;
	}
}

void Elmnt::set_dNdX()
{
	cout << "\nndN/dx matrix:\n";

	for (int NSF = 0; NSF < 4; NSF++)		//NSF = ShapeFunction number
	{
		cout << "\nN" << NSF << ":\n";
		for (int IP = 0; IP < 4; IP++)		//IP = Integration Point number
		{
			dNdX[NSF][IP] = revJacDivDetJ[IP][0][0] * shapeFun.dNdKsi[NSF][IP] + revJacDivDetJ[IP][0][1] * shapeFun.dNdEta[NSF][IP];
			cout << dNdX[NSF][IP] << "\t";
		}
	}
}

void Elmnt::set_dNdY()
{
	cout << "\nndN/dy matrix:\n";

	for (int NSF = 0; NSF < 4; NSF++)		//NSF = ShapeFunction number
	{
		cout << "\nN" << NSF << ":\n";
		for (int IP = 0; IP < 4; IP++)		//IP = Integration Point number
		{
			dNdX[NSF][IP] = revJacDivDetJ[IP][1][0] * shapeFun.dNdKsi[NSF][IP] + revJacDivDetJ[IP][1][1] * shapeFun.dNdEta[NSF][IP];
			cout << dNdX[NSF][IP] << "\t";
		}
	}
}


void Elmnt::showElement()
{
	cout << endl << nodeID[0].get_id() << "\t" << nodeID[1].get_id() << "\t" << nodeID[2].get_id() << "\t" << nodeID[3].get_id() << endl;

}
