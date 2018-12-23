#include "Elmnt.h"

using namespace std;

Elmnt::Elmnt(int id, Node n1, Node n2, Node n3, Node n4)
{
	//this->set_edgeOfGrid(edge);
	this->set_id(id);
	this->set_nodes(n1, n2, n3, n4);
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

void Elmnt::showElement()
{
	cout << nodeID[0].get_id() << "\t" << nodeID[1].get_id() << "\t" << nodeID[2].get_id() << "\t" << nodeID[3].get_id() << endl;

}
