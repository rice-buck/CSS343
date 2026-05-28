#include "graphm.h"

int main(){
    ifstream myFile("simple_graph.txt");

    GraphM test;
    test.buildGraph(myFile);

    test.findShortestPath();

    test.displayAllPaths();


    return 0;
}