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
/*bool Elmnt::is_edgeOfGrid()
{
return edgeOfGrid;
}*/
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
	/*cout << "\nTRANSFORMATION with JACOBIAN";
	for (int ip = 0; ip < 4; ip++)
	{
	cout << "\n" << ip << " integration point";
	cout << "\n1-1:\t" << transfJacobian[ip][0][0];
	cout << "\n1-2:\t" <<transfJacobian[ip][0][1];
	cout << "\n2-1:\t"<< transfJacobian[ip][1][0];
	cout << "\n2-2:\t" << transfJacobian[ip][1][1];
	}*/
}
void Elmnt::set_detJ()
{
	for (int i = 0; i < 4; i++)
	{
		this->detJ[i] = ((transfJacobian[i][0][0] * transfJacobian[i][1][1]) - (transfJacobian[i][0][1] * transfJacobian[i][1][0]));
	}
	//cout << "\ndetJ 1: " << detJ[0] << "\tdetJ 2: " << detJ[1] << "\tdetJ 3: " << detJ[2] << "\tdetJ 4: " << detJ[3] << endl;
}

void Elmnt::set_revJacDivDetJ()
{
	//cout << "\n\nreverse Jacobian divided by determinant f Jacobi matrix\n";
	for (int index = 0; index < 4; index++)
	{
		revJacDivDetJ[index][0][0] = (transfJacobian[index][1][1] / detJ[index]);
		revJacDivDetJ[index][0][1] = -(transfJacobian[index][0][1] / detJ[index]);
		revJacDivDetJ[index][1][0] = -(transfJacobian[index][1][0] / detJ[index]);
		revJacDivDetJ[index][1][1] = (transfJacobian[index][0][0] / detJ[index]);

		//cout<< index<<" integration point: \n"<< revJacDivDetJ[index][0][0]<<"\t"<<revJacDivDetJ[index][0][1];
		//cout << "\t" << revJacDivDetJ[index][1][0] << "\t" << revJacDivDetJ[index][1][1] << endl;
	}
}

void Elmnt::set_dNdX()
{
	//cout << "\nndN/dx matrix:\n";

	for (int NSF = 0; NSF < 4; NSF++)		//NSF = ShapeFunction number
	{
		//	cout << "\nN" << NSF << ":\n";
		for (int IP = 0; IP < 4; IP++)		//IP = Integration Point number
		{
			dNdX[NSF][IP] = revJacDivDetJ[IP][0][0] * shapeFun.dNdKsi[NSF][IP] + revJacDivDetJ[IP][0][1] * shapeFun.dNdEta[NSF][IP];
			//	cout << dNdX[NSF][IP] << "\t";
		}
	}
}

void Elmnt::set_dNdY()
{
	//cout << "\nndN/dy matrix:\n";

	for (int NSF = 0; NSF < 4; NSF++)		//NSF = ShapeFunction number
	{
		//	cout << "\nN" << NSF << ":\n";
		for (int IP = 0; IP < 4; IP++)		//IP = Integration Point number
		{
			dNdY[NSF][IP] = revJacDivDetJ[IP][1][0] * shapeFun.dNdKsi[NSF][IP] + revJacDivDetJ[IP][1][1] * shapeFun.dNdEta[NSF][IP];
			//cout << dNdY[NSF][IP] << "\t";
		}
	}
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

	/*cout << "\n\nAUXILIARY BOUNDARY TAB:\n";
	for (int b = 0; b < 4; b++)
	{
	cout << "boundary number: " << b + 1 << ":\n";
	for (int ip = 0; ip < 2; ip++)
	{
	cout << "node num (pc): " << ip + 1 << ":\n";
	for (int i = 0; i < 2; i++)
	{
	for (int j = 0; j < 2; j++)
	cout << auxiliaryBoundaryTab[b][ip][i][j] << "\t";
	cout << endl;
	}
	}
	}*/

	double boundaryCond[4][2][2];		//[boundary num][][]
	for (int b = 0; b < 4; b++)
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				boundaryCond[b][i][j] = (auxiliaryBoundaryTab[b][0][i][j] + auxiliaryBoundaryTab[b][1][i][j])*lenghtOfEdges[b] / 2 * int(edgeOfGrid[b]); //bc detJ=l/2

																																						 /*cout << "\n\nsums:\n";
																																						 for (int b = 0; b < 4; b++)
																																						 {
																																						 cout << "boundary number: " << b + 1 << ":\n";
																																						 for (int i = 0; i < 2; i++)
																																						 {
																																						 for (int j = 0; j < 2; j++)
																																						 cout << boundaryCond[b][i][j] << "\t";
																																						 cout << endl;
																																						 }
																																						 cout << endl;
																																						 }*/

	double boundaryH[4][4];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			boundaryH[i][j] = 0;

	for (int b = 0; b < 4; b++)
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
			{
				//cout << "\nboundaryH[" << (b + i) % 4 << "][" << (b + j) % 4 << "] += boundaryCond[" << b << "][" << i << "][" << j << "]\n";
				//cout << boundaryH[(b + i) % 4][(b + j) % 4] << " += " << boundaryCond[b][i][j] << endl;
				boundaryH[(b + i) % 4][(b + j) % 4] += boundaryCond[b][i][j];
				//cout << "=" << boundaryH[(b + i) % 4][(b + j) % 4] << endl;
			}

	/*cout << "\n\nBOUNDARY CONDITIONS\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	cout << boundaryH[i][j] << "\t";
	cout << endl;
	}*/

	/*cout << "\n\n\t\tMATRIX H\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << matrixH[i][j] << "\t";
	}
	cout << "\n";
	}*/

	//========================== ASSEMBLING MATRIX H WITH BOUNDARY CONDITIONS ======================================
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			matrixH[i][j] += boundaryH[i][j];

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

	/*delete ip1xH;
	delete ip1xH;
	delete ip2xH;
	delete ip3xH;
	delete ip4xH;
	delete ip1yH;
	delete ip2yH;
	delete ip3yH;
	delete ip4yH;
	delete ip1xHdetJ;
	delete ip2xHdetJ;
	delete ip3xHdetJ;
	delete ip4xHdetJ;
	delete ip1yHdetJ;
	delete ip2yHdetJ;
	delete ip3yHdetJ;
	delete ip4yHdetJ;
	delete ip1condH;
	delete ip2condH;
	delete ip3condH;
	delete ip4condH;*/
	//problem with delete functions
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

	/*cout << "\n\nAUXILIARY MATRIXES FOR C MATRIX\n";
	cout << "integration point number 1:\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << auxiliaryMatrix[0][i][j] << "\t";
	}
	cout << endl;
	}

	cout << "integration point number 2:\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << auxiliaryMatrix[1][i][j] << "\t";
	}
	cout << endl;
	}

	cout << "integration point number 3: \n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << auxiliaryMatrix[2][i][j] << "\t";
	}
	cout << endl;
	}
	cout << "integration point number 4:\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << auxiliaryMatrix[3][i][j] << "\t";
	}
	cout << endl;
	}

	cout << "\n\nMATRIX C:\n\n";
	for (int i = 0; i < 4; i++)
	{
	for (int j = 0; j < 4; j++)
	{
	cout << matrixC[i][j] << "\t";
	}
	cout << endl;
	}*/
}

void Elmnt::set_vectorP()
{
	cout << "\nROBJE WEKTORA\n";
	cout << alpha << "<-alpha tot->" << tot;
	for (int i = 0; i < 4; i++)
		vectorP[i] = 0;

	//int i = 0;
	//for (; i < 4; i++)
	//{
	//	if (edgeOfGrid[i] == 1)
	//		break;
	//}
	//if (i == 4)
	//	return;							//idk

	for (int i = 0; i < 4; i++)
	{
		if (!edgeOfGrid[i]) //isn't that useless? it makes a lot of zeroes actually
			continue;
		cout << "lenght: " << lenghtOfEdges[i] << endl;
		vectorP[i] = 2 * alpha*tot*lenghtOfEdges[i];

		//vectorP[i] = (0.5*(1 + (1 / sqrt(3))) + 0.5*(1 - (1 / sqrt(3))) + 0.5*(1 - (1 / sqrt(3))) + 0.5*(1 + (1 / sqrt(3))))*alpha*tot*lenghtOfEdges[i];
		cout << "\t" << vectorP[i] << "\t";
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
