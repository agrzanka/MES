#include "Grid.h"
#include "Node.h"
#include "Input_data.h"

using namespace std;

Grid::Grid(Input_data data)
{
	this->H = data.get_height();
	this->L = data.get_lenght();
	this->nH = data.get_num_of_nodes_H();
	this->nL = data.get_num_of_nodes_L();

	this->deltaY = this->H / (this->nH-1);
	this->deltaX = this->L / (this->nL-1);
	std::cout << "delta X: " << this->deltaX << "\ndeltaY: " << deltaY << std::endl;

	this->nodes = new Node*[nH];
	for (int i = 0; i < nH; i++)
		nodes[i] = new Node[nL];

	this->prepareNodes();

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
			nodes[indexH][indexL].set_id(indexH + indexL*nH);
			nodes[indexH][indexL].set_x(indexL*deltaX);
			nodes[indexH][indexL].set_y(indexH*deltaY);

			if (indexH == 0 || indexH == (nH - 1) || indexL == 0 || indexL == (nL - 1))
				nodes[indexH][indexL].set_edge(true);
			else
				nodes[indexH][indexL].set_edge(false);
		}
	}
	//this->nodes;
}

void Grid::showNodesinGrid()
{
		for (int h = nH-1; h >= 0; h--)
		{
			for (int l = 0; l < nL; l++)
				cout << nodes[h][l].get_id() << "\t";
			cout << std::endl;
		}
}