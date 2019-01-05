#pragma once

#include <iostream>
#include <fstream>
#include <string>

class Input_data {

	double H = NULL, L = NULL; //Height and Lenght of the grid  
							   //--it could be integer, but idk if there wouldn't be any double=int/int kind of problems --TO CHECK~!!!!!!!!
	double nH = NULL, nL = NULL; //number of nodes in height and in lenght
	double k = NULL; //thermal conductivity
	double tot = NULL;  //ambient temperature
	double tStart = NULL; //temperature at the beginning
	double alpha = NULL;
	double ro = NULL; //density
	double cv = NULL; //specific heat

	int numberOfNodes = NULL;
	int numberOfElements = NULL;

	//=====for non-stationary system=================
	double time = NULL; //process time
	double timeStep = NULL; //time step size


public:

	Input_data();
	~Input_data();

	double get_height();
	double get_lenght();
	double get_num_of_nodes_H();
	double get_num_of_nodes_L();
	double get_k();
	double get_tot();
	double get_tStart();
	double get_alpha();
	double get_ro();
	double get_cv();
	int get_numberOfNodes();
	int get_numberOfElements();
	double get_time();
	double get_timeStep();

	void set_height(double height);
	void set_lenght(double lenght);
	void set_num_of_nodes_H(double numnodesH);
	void set_num_of_nodes_L(double numnodesL);
	void set_k(double k);
	void set_tot(double tot);
	void set_tStart(double tStart);
	void set_alpha(double alpha);
	void set_ro(double ro);
	void set_cv(double cv);
	void set_time(double time);
	void set_timeStep(double timeStep);

};