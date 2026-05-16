//---------------------------------------------------------------------------
// driver.cpp
//---------------------------------------------------------------------------
// This code tests the basic functionality of the classes to perform
// Dijkstra's algorihms and depth-first search.
// It is not meant to exhaustively test the class. 
//
// Assumptions:
//   -- students can follow the directions to interface with this file
//   -- text files "data31.txt" and "data32.txt" are formatted as described 
//   -- Data file dataUWB provides an additional data set for part 1;
//      it must be edited, as it starts with a description how to use it
//---------------------------------------------------------------------------

#include <iostream>
#include <fstream>
//#include "graphl.h"
#include "graphm.h"
using namespace std;

int main() {

	
  cout<<"\n"<<"Part 1"<<endl;
	ifstream infile("simple_graph.txt");
	if (!infile) {
		cout << "File could not be opened." << endl;
		return 1;
	}

	//for each graph, find the shortest path from every node to all other nodes
	for (;;) {
		if (infile.eof()){
      		cout<<"No more graphs to read"<<endl;
      		break;
      	}
		
		GraphM G;
		if (G.buildGraph(infile) < 0) break;
		cout << "Find all shortest paths" << endl;
         
		G.findShortestPath();
		
		cout << "Display all shortest paths" << endl;
      	G.displayAllPaths();              // display shortest distance, path to cout


      	cout << "Display a shortest paths from data[3] to data[1]" <<endl;
		G.displayPath(3, 1);              // display path from node 3 to 1 to cout

		//Additional testing 
		cout << "\n--- Testing removes and inserts ---" << endl;

		// 1. Test Error Handling Bounds
		if (G.insertEdge(0, 3, 10) == -1) cout << "Success: Prevented invalid insertion (Node 0)" << endl;
		if (G.removeEdge(1, 99) == -1)    cout << "Success: Prevented invalid removal (Node 99)" << endl;

		// 2. Test Edge Disconnection Display
		cout << "\nTesting displayPath on a completely disconnected pair (2 -> 1):" << endl;
		G.displayPath(2, 1); // Expected: --- with no path traversal errors

		// 3. Test Graph Mutation and Re-calculation
		cout << "\nRemoving edge (1, 3) and adding cheaper edge (2, 3)" << endl;
		G.removeEdge(1, 3);
		G.insertEdge(2, 3, 5);

		cout << "Re-calculating paths with modifications:" << endl;
		G.findShortestPath();
		G.displayAllPaths(); // Visual verification that layout shifted accurately

	}
    
	
	infile.clear();
	infile.seekg(0, ios::beg);

//   cout<<"\n"<<"Part 2"<<endl;

// 	//for each graph, find the depth-first search ordering
// 	for (;;) {
// 		GraphL G;

// 		if (infile.eof()){
//       		cout<<"No more graphs to read"<<endl;
//       		break;
// 		}
		

//         if (G.buildGraph(infile) < 0)
//             break;
		

//       	cout << "Display Graph" << endl;
// 		G.displayGraph();


// 		cout << "Display edge list for node 1" << endl;
// 		cout << G.edgeList(1) << endl;
		

// 		cout << "Display DFS from node  1" << endl;
// 		cout << G.DFSorder() << endl;
// 		cout << "Display BFS from node  1" << endl;
// 		cout << G.BFSorder() << endl;


		
	
// 	}
	
	
// 	cout << endl;

  
	return 0;

}