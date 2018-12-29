#include "Grid.h"
#include "Node.h"
#include "Input_data.h"
#include "Elmnt.h"

using namespace std;

Grid::Grid(Input_data data, ShapeFunctions shapeFun)
{
	this->data = data;
	this->shapeFun = shapeFun;
	this->H = data.get_height();
	this->L = data.get_lenght();
	this->nH = data.get_num_of_nodes_H();
	this->nL = data.get_num_of_nodes_L();

	this->tot = data.get_tot();

	this->deltaY = this->H / (this->nH - 1);
	this->deltaX = this->L / (this->nL - 1);
	std::cout << "delta X: " << this->deltaX << "\ndeltaY: " << deltaY << std::endl;

	this->nodes = new Node*[nL];
	for (int i = 0; i < nL; i++)
		nodes[i] = new Node[nH];

	this->prepareNodes();

	this->gridElmnts = new Elmnt*[nL - 1];
	for (int j = 0; j < (nL - 1); j++)
		gridElmnts[j] = new Elmnt[nH - 1];

	this->prepareElements();
	this->set_globalMatrixH();
	this->show_globalMatrixH();
	this->set_globalMatrixC();
	this->show_globalMatrixC();
	this->set_globalVectorP();
	this->show_globalVectorP();
}

Grid::~Grid()
{

}
void Grid::prepareNodes()
{
	for (int indexL = 0; indexL < nL; indexL++)
	{
		for (int indexH = 0; indexH < nH; indexH++)
		{
			nodes[indexL][indexH].set_id(indexH + indexL*nH);
			nodes[indexL][indexH].set_x(indexL*deltaX);
			nodes[indexL][indexH].set_y(indexH*deltaY);

			if (indexH == 0 || indexH == (nH - 1) || indexL == 0 || indexL == (nL - 1))
				nodes[indexL][indexH].set_edge(true);
			else
				nodes[indexL][indexH].set_edge(false);
		}
	}
	//this->nodes;
}

void Grid::showNodesinGrid()
{
	for (int h = nH - 1; h >= 0; h--)
	{
		for (int l = 0; l < nL; l++)
			cout << nodes[l][h].get_id() << "\t";
		cout << std::endl;
	}
}

void Grid::showNodesEdges()
{
	for (int h = nH - 1; h >= 0; h--)
	{
		for (int l = 0; l < nL; l++)
			cout << nodes[l][h].is_edge() << "\t";
		cout << std::endl;
	}
}


void Grid::prepareElements()
{
	for (int indexL = 0; indexL < nL - 1; indexL++)
	{
		for (int indexH = 0; indexH < nH - 1; indexH++)
		{
			gridElmnts[indexL][indexH].set_id(indexH + indexL*(nH - 1));
			gridElmnts[indexL][indexH].set_constants(data);
			gridElmnts[indexL][indexH].set_nodes(nodes[indexL][indexH], nodes[indexL][indexH + 1], nodes[indexL + 1][indexH + 1], nodes[indexL + 1][indexH]);
			gridElmnts[indexL][indexH].set_edgeOfGrid();
			gridElmnts[indexL][indexH].set_lenghtOfEdges();
			gridElmnts[indexL][indexH].set_shapeFunctions(shapeFun);
			gridElmnts[indexL][indexH].set_interpolationOfCoordinates();
			gridElmnts[indexL][indexH].set_transformationJacobian();
			gridElmnts[indexL][indexH].set_detJ();
			gridElmnts[indexL][indexH].set_revJacDivDetJ();
			gridElmnts[indexL][indexH].set_dNdX();
			gridElmnts[indexL][indexH].set_dNdY();
			gridElmnts[indexL][indexH].set_matrixH();
			gridElmnts[indexL][indexH].set_matrixC();
			gridElmnts[indexL][indexH].set_vectorP();
		}
	}
}

void Grid::showElementsInGrid()
{
	for (int h = nH - 2; h >= 0; h--)
	{
		for (int l = 0; l < nL - 1; l++)
			cout << gridElmnts[h][l].get_id() << "\t";
		cout << std::endl;
	}
}

void Grid::set_globalMatrixH()
{
	this->globalMatrixH = new double*[nL*nH];
	for (int i = 0; i < nL*nH; i++)
	{
		globalMatrixH[i] = new double[nL*nH];
		for (int j = 0; j < nL*nH; j++)
			globalMatrixH[i][j] = 0;
	}

	for (int indexL = 0; indexL < nL - 1; indexL++)
	{
		for (int indexH = 0; indexH < nH - 1; indexH++)
		{
			cout << "\n(" << indexL << " , " << indexH << " )";
			int ids[4];
			ids[0] = gridElmnts[indexL][indexH].nodeID[0].get_id();
			ids[1] = gridElmnts[indexL][indexH].nodeID[1].get_id();
			ids[2] = gridElmnts[indexL][indexH].nodeID[2].get_id();
			ids[3] = gridElmnts[indexL][indexH].nodeID[3].get_id();
			cout << "\nid1: " << ids[0] << "\tid2: " << ids[1] << "\tid3: " << ids[2] << "\tid4: " << ids[3];
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					globalMatrixH[ids[i]][ids[j]] += gridElmnts[indexL][indexH].matrixH[i][j];
				}
			}
		}
	}
}

void Grid::set_globalMatrixC()
{
	this->globalMatrixC = new double*[nL*nH];
	for (int i = 0; i < nL*nH; i++)
	{
		globalMatrixC[i] = new double[nL*nH];
		for (int j = 0; j < nL*nH; j++)
			globalMatrixC[i][j] = 0;
	}

	for (int indexL = 0; indexL < nL - 1; indexL++)
	{
		for (int indexH = 0; indexH < nH - 1; indexH++)
		{
			int ids[4];
			ids[0] = gridElmnts[indexL][indexH].nodeID[0].get_id();
			ids[1] = gridElmnts[indexL][indexH].nodeID[1].get_id();
			ids[2] = gridElmnts[indexL][indexH].nodeID[2].get_id();
			ids[3] = gridElmnts[indexL][indexH].nodeID[3].get_id();
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					globalMatrixC[ids[i]][ids[j]] += gridElmnts[indexL][indexH].matrixC[i][j];
				}
			}
		}
	}
}

void Grid::set_globalVectorP()
{
	this->globalVectorP = new double[(nL*nH)];
	for (int i = 0; i < nL*nH; i++)
		globalVectorP[i] = 0;

	for (int indexL = 0; indexL < nL - 1; indexL++)
	{
		for (int indexH = 0; indexH < nH - 1; indexH++)
		{
			cout << "\nvector P for ( " << indexL << " , " << indexH << " ) element: \n";
			for (int i = 0; i < 4; i++)
			{
				cout << gridElmnts[indexL][indexH].vectorP[i] << "\t";
			}
			int ids[4];
			ids[0] = gridElmnts[indexL][indexH].nodeID[0].get_id();
			ids[1] = gridElmnts[indexL][indexH].nodeID[1].get_id();
			ids[2] = gridElmnts[indexL][indexH].nodeID[2].get_id();
			ids[3] = gridElmnts[indexL][indexH].nodeID[3].get_id();
			for (int i = 0; i < 4; i++)
			{
				globalVectorP[ids[i]] += gridElmnts[indexL][indexH].vectorP[i];
				//for (int j = 0; j < 4; j++)
				//{
				//	globalMatrixC[ids[i]][ids[j]] += gridElmnts[indexL][indexH].matrixC[i][j];
				//}
			}
		}
	}

}

void Grid::show_globalMatrixH()
{
	cout << "\n\tGLOBAL MATRIX H:\n\n";
	for (int i = 0; i < nL*nH; i++)
	{
		for (int j = 0; j < nL*nH; j++)
			cout << globalMatrixH[i][j] << "\t";
		cout << endl;
	}

}
void Grid::show_globalMatrixC()
{
	cout << "\n\tGLOBAL MATRIX C:\n\n";
	for (int i = 0; i < nL*nH; i++)
	{
		for (int j = 0; j < nL*nH; j++)
			cout << globalMatrixC[i][j] << "\t";
		cout << endl;
	}
}

void Grid::show_globalVectorP()
{
	cout << "\n\tGLOBAL VECTOR P:\n\n";
	for (int i = 0; i < nL*nH; i++)
	{
		cout << globalVectorP[i] << "\t";
	}
}