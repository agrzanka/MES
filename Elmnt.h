#pragma once

#include "Input_data.h"
#include "Node.h"
#include "ShapeFunctions.h"

class Elmnt {
public:
	int id = NULL;
	Node nodeID[4];
	ShapeFunctions shapeFun;
	//interpolation of coordinates:
	double xp[4], yp[4];

	double transfJacobian[4][2][2]; //coordinates transformation with Jacobian -four matrixes- [integration point][0:ksi, 1:eta][0:x, 1:y]
	double detJ[4];//det[J] -determinants of Jacobian matrixes above, for each of integration points  [integration point]
	double revJacDivDetJ[4][2][2]; //reverse transfJacobian matrix divided by corresponding determinant of Jacobian matrix

	//bool edgeOfGrid=NULL;

	//public:

	Elmnt(int id, Node n1, Node n2, Node n3, Node n4, ShapeFunctions shapeFun);
	Elmnt();
	~Elmnt();

	void set_id(int id);
	void set_nodes(Node n1, Node n2, Node n3, Node n4);
	//void set_edgeOfGrid(bool edge);


	int get_id();
	//bool is_edgeOfGrid();


	void set_shapeFunctions(ShapeFunctions shapeFun);
	void set_interpolationOfCoordinates();

	void set_transformationJacobian();
	void set_detJ();
	void set_revJacDivDetJ();

	void showElement();
};
