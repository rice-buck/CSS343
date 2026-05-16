#include "graphl.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(){
    ifstream myFile("simple_graph.txt");

    GraphL test;

    test.buildGraph(myFile);

    test.displayGraph();

    cout << test.DFSorder() << endl;


    return 0;
}