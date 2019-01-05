#include "Input_data.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Input_data::Input_data()
{
	string line;
	ifstream datafile;
	datafile.open("data.txt");
	if (datafile.is_open())
	{
		getline(datafile, line);
		H = stod(line);
		getline(datafile, line);
		L = stod(line);
		getline(datafile, line);
		nH = stod(line);
		getline(datafile, line);
		nL = stod(line);
		getline(datafile, line);
		k = stod(line);
		getline(datafile, line);
		tot = stod(line);
		getline(datafile, line);
		tStart = stod(line);
		getline(datafile, line);
		alpha = stod(line);
		getline(datafile, line);
		ro = stod(line);
		getline(datafile, line);
		cv = stod(line);

		numberOfNodes = nH*nL;
		numberOfElements = (nH - 1)*(nL - 1);

		getline(datafile, line);
		time = stod(line);
		getline(datafile, line);
		timeStep = stod(line);

		datafile.close();

		cout << "\nH: " << H << "\tL: " << L;
		cout << "\nnH: " << nH << "\tnL: " << nL;
		cout << "\nwspolczynnik k: " << k << "alfa: " << alpha << "\ngestosc: " << ro << "\cieplo wlasciwe: " << cv;
		cout << "\ntemperatura otoczenia: " << tot << "\ntemperatura poczatkowa: " << tStart;
		cout << "\nczas: " << time << "\nkrok czasowy: " << timeStep << endl;

		cout << "\nliczba wezlow w siatce: " << numberOfNodes << endl;
		cout << "liczba elementow w siatce: " << numberOfElements << endl;
	}
	else
		cout << "unnable to open the data file :( ";
}

Input_data::~Input_data()
{
	H = NULL, L = NULL, nH = NULL, nL = NULL, k = NULL, alpha = NULL, cv = NULL;
	tot = NULL, tStart = NULL, time = NULL, timeStep = NULL;
	numberOfElements = NULL; numberOfNodes = NULL;
}

double Input_data::get_height()
{
	return H;
}
double Input_data::get_lenght()
{
	return L;
}
double Input_data::get_num_of_nodes_H()
{
	return nH;
}
double Input_data::get_num_of_nodes_L()
{
	return nL;
}
double Input_data::get_k()
{
	return k;
}
double Input_data::get_tot()
{
	return tot;
}
double Input_data::get_tStart()
{
	return tStart;
}
double Input_data::get_alpha()
{
	return alpha;
}
double Input_data::get_ro()
{
	return ro;
}
double Input_data::get_cv()
{
	return cv;
}
int Input_data::get_numberOfNodes()
{
	return numberOfNodes;
}
int Input_data::get_numberOfElements()
{
	return numberOfElements;
}
double Input_data::get_time()
{
	return time;
}
double Input_data::get_timeStep()
{
	return timeStep;
}

//setters from different source
void Input_data::set_height(double height)
{
	this->H = height;
}
void Input_data::set_lenght(double lenght)
{
	this->L = lenght;
}
void Input_data::set_num_of_nodes_H(double numnodesH)
{
	this->nH = numnodesH;
}
void Input_data::set_num_of_nodes_L(double numnodesL)
{
	this->nL = numnodesL;
}
void Input_data::set_k(double k)
{
	this->k = k;
}
void Input_data::set_tot(double tot)
{
	this->tot = tot;
}
void Input_data::set_tStart(double tStart)
{
	this->tStart = tStart;
}
void Input_data::set_alpha(double alpha)
{
	this->alpha = alpha;
}
void Input_data::set_ro(double ro)
{
	this->ro = ro;
}
void Input_data::set_cv(double cv)
{
	this->cv = cv;
}
void Input_data::set_time(double time)
{
	this->time = time;
}
void Input_data::set_timeStep(double timeStep)
{
	this->timeStep = timeStep;
}