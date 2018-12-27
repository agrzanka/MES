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
	this->set_matrixH();
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

void Elmnt::set_matrixH()
{
	//=========== auxiliary tabs ================== 
	//matrixes below: vector{dN/dx} multiplied by transposed vector {dN/dx} ip1 stands for Integration point no.1 etc.
	double ip1xH[4][4];
	double ip2xH[4][4];
	double ip3xH[4][4];
	double ip4xH[4][4];
	//matrixes below: vector{dN/dy} multiplied by transposed vector {dN/dy} ip1 stands for Integration point no.1 etc.
	double ip1yH[4][4];
	double ip2yH[4][4];
	double ip3yH[4][4];
	double ip4yH[4][4];
	//matrixes below: matrixes from above multiplied by detJ
	double ip1xHdetJ[4][4];
	double ip2xHdetJ[4][4];
	double ip3xHdetJ[4][4];
	double ip4xHdetJ[4][4];
	double ip1yHdetJ[4][4];
	double ip2yHdetJ[4][4];
	double ip3yHdetJ[4][4];
	double ip4yHdetJ[4][4];
	//matrixes below: conductivity*({dN/dx}*{dN/dx}Transposed*detJ + {dN/dy}*{dN/dy}Transposed*detJ)
	double ip1condH[4][4];
	double ip2condH[4][4];
	double ip3condH[4][4];
	double ip4condH[4][4];

	//========== filling auxiliary tabs ==============
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ip1xH[i][j] = dNdX[i][0] * dNdX[j][0];
			//cout << "\n" << dNdX[i][0] << " * " << dNdX[j][0];
			ip2xH[i][j] = dNdX[i][1] * dNdX[j][1];
			ip3xH[i][j] = dNdX[i][2] * dNdX[j][2];
			ip4xH[i][j] = dNdX[i][3] * dNdX[j][3];

			ip1yH[i][j] = dNdY[i][0] * dNdY[j][0];
			ip2yH[i][j] = dNdY[i][1] * dNdY[j][1];
			ip3yH[i][j] = dNdY[i][2] * dNdY[j][2];
			ip4yH[i][j] = dNdY[i][3] * dNdY[j][3];

			ip1xHdetJ[i][j] = ip1xH[i][j] * detJ[0];
			ip2xHdetJ[i][j] = ip2xH[i][j] * detJ[1];
			ip3xHdetJ[i][j] = ip3xH[i][j] * detJ[2];
			ip4xHdetJ[i][j] = ip4xH[i][j] * detJ[3];

			ip1yHdetJ[i][j] = ip1yH[i][j] * detJ[0];
			ip2yHdetJ[i][j] = ip2yH[i][j] * detJ[1];
			ip3yHdetJ[i][j] = ip3yH[i][j] * detJ[2];
			ip4yHdetJ[i][j] = ip4yH[i][j] * detJ[3];

			ip1condH[i][j] = k*(ip1xHdetJ[i][j] + ip1yHdetJ[i][j]);
			ip2condH[i][j] = k*(ip2xHdetJ[i][j] + ip2yHdetJ[i][j]);
			ip3condH[i][j] = k*(ip3xHdetJ[i][j] + ip3yHdetJ[i][j]);
			ip4condH[i][j] = k*(ip4xHdetJ[i][j] + ip4yHdetJ[i][j]);

			matrixH[i][j] = ip1condH[i][j] + ip2condH[i][j] + ip3condH[i][j] + ip4condH[i][j];
		}
	}

	cout << "\n\n\t\tMATRIX H\n";
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << matrixH[i][j] << "\t";
		}
		cout << "\n";
	}

	/*cout << "\nIP=1, {dN/dx}*{dN/dx}Transposed:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip1xH[i][j] << "\t";
	}
	cout << "\n";
	}

	cout << "\nIP=2, {dN/dx}*{dN/dx}Transposed:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip2xH[i][j] << "\t";
	}
	cout << endl;
	}

	cout << "\nIP=3, {dN/dx}*{dN/dx}Transposed:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip3xH[i][j] << "\t";
	}
	cout << endl;
	}

	cout << "\nIP=4, {dN/dx}*{dN/dx}Transposed:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip4xH[i][j] << "\t";
	}
	cout << endl;
	}*/

	/*cout << "\nIP=1, {dN/dy}*{dN/dy}Transposed:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip1yH[i][j] << "\t";
	}
	cout << "\n";
	}

	cout << "\nIP=2, {dN/dy}*{dN/dy}Transposed:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip2yH[i][j] << "\t";
	}
	cout << endl;
	}

	cout << "\nIP=3, {dN/dy}*{dN/dy}Transposed:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip3yH[i][j] << "\t";
	}
	cout << endl;
	}

	cout << "\nIP=4, {dN/dy}*{dN/dy}Transposed:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip4yH[i][j] << "\t";
	}
	cout << endl;
	}*/

	/*cout << "\nIP=1, {dN/dx}*{dN/dx}Transposed*detJ:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip1xHdetJ[i][j] << "\t";
	}
	cout << "\n";
	}

	cout << "\nIP=2, {dN/dx}*{dN/dx}Transposed*detJ:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip2xHdetJ[i][j] << "\t";
	}
	cout << endl;
	}

	cout << "\nIP=3, {dN/dx}*{dN/dx}Transposed*detJ:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip3xHdetJ[i][j] << "\t";
	}
	cout << endl;
	}

	cout << "\nIP=4, {dN/dx}*{dN/dx}Transposed*detJ:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip4xHdetJ[i][j] << "\t";
	}
	cout << endl;
	}

	cout << "\nIP=1, {dN/dy}*{dN/dy}Transposed*detJ:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip1yHdetJ[i][j] << "\t";
	}
	cout << "\n";
	}

	cout << "\nIP=2, {dN/dy}*{dN/dy}Transposed*detJ:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip2yHdetJ[i][j] << "\t";
	}
	cout << endl;
	}

	cout << "\nIP=3, {dN/dy}*{dN/dy}Transposed*detJ:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip3yHdetJ[i][j] << "\t";
	}
	cout << endl;
	}

	cout << "\nIP=4, {dN/dy}*{dN/dy}Transposed*detJ:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip4yHdetJ[i][j] << "\t";
	}
	cout << endl;
	}*/

	/*cout << "\nIP=1, conductivity*({dN/dx}*{dN/dx}Transposed*detJ + {dN/dy}*{dN/dy}Transposed*detJ:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip1condH[i][j] << "\t";
	}
	cout << "\n";
	}

	cout << "\nIP=2, conductivity*({dN/dx}*{dN/dx}Transposed*detJ + {dN/dy}*{dN/dy}Transposed*detJ:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip2condH[i][j] << "\t";
	}
	cout << endl;
	}

	cout << "\nIP=3, conductivity*({dN/dx}*{dN/dx}Transposed*detJ + {dN/dy}*{dN/dy}Transposed*detJ:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip3condH[i][j] << "\t";
	}
	cout << endl;
	}

	cout << "\nIP=4, conductivity*({dN/dx}*{dN/dx}Transposed*detJ + {dN/dy}*{dN/dy}Transposed*detJ:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << ip4condH[i][j] << "\t";
	}
	cout << endl;
	}*/

	//delete ip1xH, ip1xH, ip2xH, ip3xH, ip4xH, ip1yH, ip2yH, ip3yH, ip4yH;
	//delete ip1xHdetJ, ip2xHdetJ, ip3xHdetJ, ip4xHdetJ, ip1yHdetJ, ip2yHdetJ, ip3yHdetJ, ip4yHdetJ;
}

void Elmnt::showElement()
{
	cout << endl << nodeID[0].get_id() << "\t" << nodeID[1].get_id() << "\t" << nodeID[2].get_id() << "\t" << nodeID[3].get_id() << endl;

}
