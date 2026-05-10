#include "graphm.h"
#include <fstream>
#include <sstream>

#define INF 100 //one billion

GraphM::GraphM(){
    size = 0;

    //intialize vertices
    for(int i = 0; i < MAXNODES; ++i){
        vertices[i] = "";
    }

    //All entries of AdjM set to infinity
    for(int i = 0; i < MAXNODES; ++i){
        for(int j = 0; j < MAXNODES; ++j){
            AdjM[i][j] = INF;
        }
    }

    // All entires of PathM is initialized to sets all dist to infinity, 
    // sets all visited to false, and sets all prev_node to 0.
    for(int i = 0; i < MAXNODES; ++i){
        for(int j = 0; j < MAXNODES; ++j){
            PathM[i][j].dist = INF;
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
    
    //MAXNODES = size ??
    
    //read size
    file >> size;
    //for tesing 
    cout << size;

    string line; 

    for(int i = 0; i <= size; ++i){
        getline(file, line);
        cout << line << endl;
        vertices[i] = line;
    }


    //read vertices
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int from, to, dist;
        
        if(ss >> from >> to >> dist){

            if(from == 0 && to == 0 && dist == 0) break;
            AdjM[from][to] = dist;
        }
    }

    file.close();

    

    return 1;
}