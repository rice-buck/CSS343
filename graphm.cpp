#include "graphm.h"
#include <fstream>

#define INFINITY 1e9 //one billion

GraphM::GraphM(){
    size = 0;

    //intialize vertices
    for(int i = 0; i < MAXNODES; ++i){
        vertices[i] = "";
    }

    //All entries of AdjM set to infinity
    for(int i = 0; i < MAXNODES; ++i){
        for(int j = 0; j < MAXNODES; ++j){
            AdjM[i][j] = INFINITY;
        }
    }

    // All entires of PathM is initialized to sets all dist to infinity, 
    // sets all visited to false, and sets all prev_node to 0.
    for(int i = 0; i < MAXNODES; ++i){
        for(int j = 0; j < MAXNODES; ++j){
            PathM[i][j].dist = INFINITY;
            PathM[i][j].visited = false;
            PathM[i][j].prev_node = 0;
        }
    }
}

int GraphM::buildGraph(ifstream& file){
    if(!file) {
        std::cerr << "Unable to open file!";
        return -1;
    }

    //read size
    file >> size;
    cout << size;

    //MAXNODES = size ?

    //read vertices
    //file.ignore() //need to ignore \n new lines
    file.close();
    return 1;
}