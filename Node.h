#pragma once

#include "Input_data.h"

class Node {
public:
	int id;
	double x, y, t;
	bool edge;

	Node(int id, double x, double y, double t, bool edge);
	Node();
	~Node();

	void set_x(double x);
	void set_y(double y);
	void set_id(int id);
	void set_edge(bool edge);
	void set_temperature(double t);

	int get_id();
	bool is_edge();
	double get_x();
	double get_y();
	double get_temperature();
};