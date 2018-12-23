#pragma once

#include "Input_data.h"
#include "Node.h"

class Elmnt {
public:
	int id = NULL;
	Node nodeID[4];
	//bool edgeOfGrid=NULL;

	//public:

	Elmnt(int id, Node n1, Node n2, Node n3, Node n4);
	Elmnt();
	~Elmnt();

	void set_id(int id);
	void set_nodes(Node n1, Node n2, Node n3, Node n4);
	//void set_edgeOfGrid(bool edge);


	int get_id();
	//bool is_edgeOfGrid();

	void showElement();
};
