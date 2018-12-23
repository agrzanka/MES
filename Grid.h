#pragma once
#include "Node.h"
#include "Input_data.h"

class Grid {
	//Node **nodes = NULL;
	//matrix of elements needed here

	double H = NULL;
	double L = NULL;
	short nH = NULL;
	short nL = NULL;
	double deltaX = NULL;
	double deltaY = NULL;
	//insert the rest of needed data here

public:
	Node **nodes = NULL;
	Grid(Input_data data);
	~Grid();

	void prepareNodes();
	void showNodesinGrid();
};