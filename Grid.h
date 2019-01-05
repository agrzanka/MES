#pragma once
#include "Node.h"
#include "Input_data.h"
#include "Elmnt.h"

class Grid {
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

	double timeStep = NULL;


public:
	Elmnt**gridElmnts = NULL;
	Node **nodes = NULL;

	double*vectorTemp;

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

	void clearAll();

	void init_vectorTemp();
	void set_temp(double* vector);
	double*get_temp();

	void divCbyTimeStep(double dT);
	void addCdivTimeStep2H();
	void addCdivTimeStepmultTemp2P();

	void show_addCdivTimeStep2H();
	void show_addCdivTimeStepmultTemp2P();
};
