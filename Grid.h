#pragma once
#include "Node.h"
#include "Input_data.h"
#include "Elmnt.h"


class Grid {
	//Node **nodes = NULL;
	//matrix of elements needed here
	Input_data data;

	double H = NULL;
	double L = NULL;
	short nH = NULL;
	short nL = NULL;
	double deltaX = NULL;
	double deltaY = NULL;

	ShapeFunctions shapeFun;
	double tot = NULL;
	double k = NULL;
	double c = NULL;
	double alpha = NULL;
	double ro = NULL;


public:
	Elmnt**gridElmnts = NULL;
	Node **nodes = NULL;

	double **globalMatrixH;
	double **globalMatrixC;
	double *globalVectorP;

	Grid(Input_data data, ShapeFunctions shapeFun);
	~Grid();

	void prepareNodes();
	void showNodesinGrid();
	void showNodesEdges();

	void prepareElements();
	void showElementsInGrid();

	void set_globalMatrixH();
	void set_globalMatrixC();
	void set_globalVectorP();

	void show_globalMatrixH();
	void show_globalMatrixC();
	void show_globalVectorP();
};
