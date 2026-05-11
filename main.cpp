#include "graphm.h"

int main(){
    ifstream myFile("simple_graph.txt");

    GraphM test;
    if(test.buildGraph(myFile)) cout << "\nSuccess!" << endl;

    test.displayAllPaths();


    return 0;
}