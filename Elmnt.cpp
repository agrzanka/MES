#include "Elmnt.h"
#include <math.h>

using namespace std;

Elmnt::Elmnt(int id, Node n1, Node n2, Node n3, Node n4, ShapeFunctions shapeFun, Input_data data)
{
	this->set_id(id);
	this->set_constants(data);
	this->set_nodes(n1, n2, n3, n4);
	this->set_edgeOfGrid();
	this->set_lenghtOfEdges();
	this->set_shapeFunctions(shapeFun);
	this->set_interpolationOfCoordinates();
	this->set_transformationJacobian();
	this->set_detJ();
	this->set_revJacDivDetJ();
	this->set_dNdX();
	this->set_dNdY();
	this->set_matrixH();
	this->set_matrixC();
	this->set_vectorP();
}

Elmnt::Elmnt()
{

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
void Elmnt::set_edgeOfGrid()
{
	for (int i = 0; i < 3; i++)
		edgeOfGrid[i] = nodeID[i].edge && nodeID[i + 1].edge;
	edgeOfGrid[3] = nodeID[3].edge && nodeID[0].edge;
}

void Elmnt::set_lenghtOfEdges()
{
	lenghtOfEdges[0] = fabs(nodeID[0].get_x() - nodeID[1].get_x());
	lenghtOfEdges[1] = fabs(nodeID[1].get_y() - nodeID[2].get_y());
	lenghtOfEdges[2] = fabs(nodeID[2].get_x() - nodeID[3].get_x());
	lenghtOfEdges[3] = fabs(nodeID[3].get_y() - nodeID[0].get_y());
}

int Elmnt::get_id()
{
	return id;
}

void Elmnt::set_constants(Input_data data)
{
	this->tot = data.get_tot();
	this->alpha = data.get_alpha();
	this->cv = data.get_cv();
	this->k = data.get_k();
	this->ro = data.get_ro();
	this->tStart = data.get_tStart();
	this->time = data.get_time();
	this->timeStep = data.get_timeStep();
}

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
}
void Elmnt::set_detJ()
{
	for (int i = 0; i < 4; i++)
		this->detJ[i] = ((transfJacobian[i][0][0] * transfJacobian[i][1][1]) - (transfJacobian[i][0][1] * transfJacobian[i][1][0]));
}

void Elmnt::set_revJacDivDetJ()
{
	//reverse Jacobian divided by determinant of Jacobi matrix
	for (int index = 0; index < 4; index++)
	{
		revJacDivDetJ[index][0][0] = (transfJacobian[index][1][1] / detJ[index]);
		revJacDivDetJ[index][0][1] = -(transfJacobian[index][0][1] / detJ[index]);
		revJacDivDetJ[index][1][0] = -(transfJacobian[index][1][0] / detJ[index]);
		revJacDivDetJ[index][1][1] = (transfJacobian[index][0][0] / detJ[index]);
	}
}

void Elmnt::set_dNdX()
{
	for (int NSF = 0; NSF < 4; NSF++)		//NSF = ShapeFunction number
		for (int IP = 0; IP < 4; IP++)		//IP = Integration Point number
			dNdX[NSF][IP] = revJacDivDetJ[IP][0][0] * shapeFun.dNdKsi[NSF][IP] + revJacDivDetJ[IP][0][1] * shapeFun.dNdEta[NSF][IP];
}

void Elmnt::set_dNdY()
{
	for (int NSF = 0; NSF < 4; NSF++)		//NSF = ShapeFunction number
		for (int IP = 0; IP < 4; IP++)		//IP = Integration Point number
			dNdY[NSF][IP] = revJacDivDetJ[IP][1][0] * shapeFun.dNdKsi[NSF][IP] + revJacDivDetJ[IP][1][1] * shapeFun.dNdEta[NSF][IP];
}

void Elmnt::set_matrixH()
{
	//=========== auxiliary tabs ================== 
	//matrixes below: vector{dN/dx} multiplied by transposed vector {dN/dx} ip1 stands for Integration point no.1 etc.
	double** ip1xH = new double *[4];
	double **ip2xH = new double*[4];
	double** ip3xH = new double*[4];
	double** ip4xH = new double*[4];
	for (int i = 0; i < 4; i++)
	{
		ip1xH[i] = new double[4];
		ip2xH[i] = new double[4];
		ip3xH[i] = new double[4];
		ip4xH[i] = new double[4];
	}
	//matrixes below: vector{dN/dy} multiplied by transposed vector {dN/dy} ip1 stands for Integration point no.1 etc.
	double** ip1yH = new double *[4];
	double** ip2yH = new double*[4];
	double** ip3yH = new double*[4];
	double** ip4yH = new double*[4];
	for (int i = 0; i < 4; i++)
	{
		ip1yH[i] = new double[4];
		ip2yH[i] = new double[4];
		ip3yH[i] = new double[4];
		ip4yH[i] = new double[4];
	}
	//matrixes below: matrixes from above multiplied by detJ
	double** ip1xHdetJ = new double*[4];
	double** ip2xHdetJ = new double*[4];
	double** ip3xHdetJ = new double*[4];
	double** ip4xHdetJ = new double*[4];
	double** ip1yHdetJ = new double*[4];
	double** ip2yHdetJ = new double*[4];
	double** ip3yHdetJ = new double*[4];
	double** ip4yHdetJ = new double*[4];
	for (int i = 0; i < 4; i++)
	{
		ip1xHdetJ[i] = new double[4];
		ip2xHdetJ[i] = new double[4];
		ip3xHdetJ[i] = new double[4];
		ip4xHdetJ[i] = new double[4];
		ip1yHdetJ[i] = new double[4];
		ip2yHdetJ[i] = new double[4];
		ip3yHdetJ[i] = new double[4];
		ip4yHdetJ[i] = new double[4];
	}
	//matrixes below: conductivity*({dN/dx}*{dN/dx}Transposed*detJ + {dN/dy}*{dN/dy}Transposed*detJ)
	double** ip1condH = new double*[4];
	double** ip2condH = new double*[4];
	double** ip3condH = new double*[4];
	double** ip4condH = new double*[4];
	for (int i = 0; i < 4; i++)
	{
		ip1condH[i] = new double[4];
		ip2condH[i] = new double[4];
		ip3condH[i] = new double[4];
		ip4condH[i] = new double[4];
	}

	//========== filling auxiliary tabs ==============
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ip1xH[i][j] = dNdX[i][0] * dNdX[j][0];
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

	//================= ADDING BOUNDARY CONDITIONS ================================
	//auxuliary tab for boundary conditions

	cout << "\n\nEDGES:\n";
	for (int i = 0; i < 4; i++)
	{
		cout << edgeOfGrid[i] << "\t";
	}

	double boundaryN[2][2];		// [node num(0/1)][shape fun num]
	boundaryN[0][0] = 0.5*(1 + (1 / sqrt(3)));
	boundaryN[0][1] = 0.5*(1 - (1 / sqrt(3)));
	boundaryN[1][0] = 0.5*(1 - (1 / sqrt(3)));
	boundaryN[1][1] = 0.5*(1 + (1 / sqrt(3)));

	double auxiliaryBoundaryTab[4][2][2][2];	// [boundary num][node num(0/1)][][]
	for (int b = 0; b < 4; b++)
		for (int ip = 0; ip < 2; ip++)
			for (int i = 0; i < 2; i++)
				for (int j = 0; j < 2; j++)
					auxiliaryBoundaryTab[b][ip][i][j] = boundaryN[ip][i] * boundaryN[ip][j] * alpha;


	double boundaryCond[4][2][2];		//[boundary num][][]
	for (int b = 0; b < 4; b++)
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				boundaryCond[b][i][j] = (auxiliaryBoundaryTab[b][0][i][j] + auxiliaryBoundaryTab[b][1][i][j])*lenghtOfEdges[b] / 2 * int(edgeOfGrid[b]); //bc detJ=l/2																																		 }*/


	double boundaryH[4][4];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			boundaryH[i][j] = 0;

	for (int b = 0; b < 4; b++)
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				boundaryH[(b + i) % 4][(b + j) % 4] += boundaryCond[b][i][j];


	//========================== ASSEMBLING MATRIX H WITH BOUNDARY CONDITIONS ======================================
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			matrixH[i][j] += boundaryH[i][j];
}

void Elmnt::set_matrixC()
{
	double auxiliaryMatrix[4][4][4]; //[integration point][][]

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			auxiliaryMatrix[0][i][j] = shapeFun.N[i][0] * shapeFun.N[j][0] * detJ[0] * cv*ro;
			auxiliaryMatrix[1][i][j] = shapeFun.N[i][1] * shapeFun.N[j][1] * detJ[1] * cv*ro;
			auxiliaryMatrix[2][i][j] = shapeFun.N[i][2] * shapeFun.N[j][2] * detJ[2] * cv*ro;
			auxiliaryMatrix[3][i][j] = shapeFun.N[i][3] * shapeFun.N[j][3] * detJ[3] * cv*ro;
			matrixC[i][j] = auxiliaryMatrix[0][i][j] + auxiliaryMatrix[1][i][j] + auxiliaryMatrix[2][i][j] + auxiliaryMatrix[3][i][j];
		}
	}
}

void Elmnt::set_vectorP()
{
	for (int i = 0; i < 4; i++)
		vectorP[i] = 0;

	int i = 0;
	for (; i < 4; i++)
	{
		if (edgeOfGrid[i] == 1)
			break;
	}
	if (i == 4)
		return;

	for (int i = 0; i < 4; i++)
	{
		if (!edgeOfGrid[i])
			continue;
		vectorP[i] = 2 * alpha*tot*lenghtOfEdges[i] / 2;
		//vectorP[i] = (0.5*(1 + (1 / sqrt(3))) + 0.5*(1 - (1 / sqrt(3))) + 0.5*(1 - (1 / sqrt(3))) + 0.5*(1 + (1 / sqrt(3))))*alpha*tot*lenghtOfEdges[i]/2;
	}
}

void Elmnt::clearHPC()
{
	for (int i = 0; i < 4; i++)
	{
		vectorP[i] = 0;
		for (int j = 0; j < 4; j++)
		{
			matrixH[i][j] = 0;
			matrixC[i][j] = 0;
		}
	}
}


void Elmnt::showElement()
{
	cout << endl << nodeID[0].get_id() << "\t" << nodeID[1].get_id() << "\t" << nodeID[2].get_id() << "\t" << nodeID[3].get_id() << endl;
}
