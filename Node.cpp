#include "Node.h"

Node::Node(int id, double x, double y, double t, bool edge)
{
	/*this->id = id;
	this->x = x;
	this->y = y;
	this->t = t;
	this->edge = edge;*/

	this->set_edge(edge);
	this->set_id(id);
	this->set_temperature(t);
	this->set_x(x);
	this->set_y(y);
}

Node::Node()
{
}

Node::~Node()
{

}

void Node::set_x(double x)
{
	this->x = x;
}
void Node::set_y(double y)
{
	this->y = y;
}
void Node::set_id(int id)
{
	this->id = id;
}
void Node::set_edge(bool edge)
{
	this->edge = edge;
}
void Node::set_temperature(double t)
{
	this->t = t;
}

int Node::get_id()
{
	return id;
}
bool Node::is_edge()
{
	return edge;
}
double Node::get_x()
{
	return x;
}
double Node::get_y()
{
	return y;
}
double Node::get_temperature()
{
	return t;
}