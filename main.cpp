#include <iostream>
#include "Input_data.h"
#include "Node.h"
#include "Grid.h"
#include "Elmnt.h"
#include "IntegrationPoints.h"
#include "ShapeFunctions.h"

using namespace std;

int main()
{
	Input_data data;

	double t = data.get_tot();

	IntegrationPoints intPoints;
	ShapeFunctions ShapeFun(intPoints);

	//	ShapeFun.showShapeFunctions();

	Grid mesh(data, ShapeFun);
	//	cout << "first one:" << endl;
	//	cout << "id: " << mesh.nodes[0][0].get_id() << "\tx: " << mesh.nodes[0][0].get_x() << endl;
	//	cout << "last one: " << endl;
	//	cout << "id: " << mesh.nodes[int(data.get_num_of_nodes_H() - 1)][int(data.get_num_of_nodes_L() - 1)].get_id() << "\tx: " 
	//		<< mesh.nodes[int(data.get_num_of_nodes_H() - 1)][int(data.get_num_of_nodes_L() - 1)].get_x() <<
	//		"\ty: " << mesh.nodes[int(data.get_num_of_nodes_H() - 1)][int(data.get_num_of_nodes_L() - 1)].get_y() << endl;


	//	cout << "id: " << mesh.nodes[2][3].get_id() << "\tx: " << mesh.nodes[2][3].get_x() << "\ty:  " << mesh.nodes[2][3].get_y() << endl;
	//	cout << "id: " << mesh.nodes[5][2].get_id() << "\tx: " << mesh.nodes[5][2].get_x() << "\ty: " << mesh.nodes[5][2].get_y() << endl;

	//	cout << "\n\nwhole grid shown by nodes\n" << endl;
	//	mesh.showNodesinGrid();

	//	cout << "\nedges\n";
	//	mesh.showNodesEdges();

	//	cout << "\n\nwhole grid by elements id\n";
	//	mesh.showElementsInGrid();

	/*	cout << "\n\nshow 0 element\n";
	mesh.gridElmnts[0][0].showElement();
	cout << "\nshow 1st element\n";
	mesh.gridElmnts[1][0].showElement();
	cout << "\nshow 4th element\n";
	mesh.gridElmnts[4][0].showElement();
	cout << "\nshow 5th element:\n";
	mesh.gridElmnts[0][1].showElement();
	cout << "\nshow 9th elemeny: \n";
	mesh.gridElmnts[4][1].showElement();
	cout << "\nshow 10th element:\n";
	mesh.gridElmnts[0][2].showElement();
	cout << "\nshow 14th element: \n";
	mesh.gridElmnts[4][2].showElement();
	*/

	Node node1(0, 0, 0, 0, 1), node2(1, 0.025, 0, 0, 1), node3(2, 0.025, 0.025, 0, 0), node4(3, 0, 0.025, 0, 0);
	Elmnt elmnt(0, node1, node2, node3, node4, ShapeFun, data);
	elmnt.showElement();


	cout << "\nSETUJ SIE PROSZEEEE: ";
	cout << mesh.nodes[0][3].get_temperature();


	system("PAUSE");
}