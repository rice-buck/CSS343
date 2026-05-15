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
        int from, to, cost;
        
        if(ss >> from >> to >> cost){

            if(from == 0 && to == 0 && cost == 0) break;
            AdjM[from][to] = cost;
        }
    }

    file.close();

    return 1;
}

//insert edge
int GraphM::insertEdge(int from, int to, int cost){
    //invalid input if from and to are less than or equal to zero, the vertices greater than size, or if the cost is negative
    if(size < from || from <= 0 || size < to || to <= 0 || cost <= 0) return -1;

    AdjM[from][to] = cost;

    return 1;
}

//set node edge cost to INF to remove it
int GraphM::removeEdge(int from, int to){
    if(size < from || from <= 0 || size < to || to <= 0) return -1;

    AdjM[from][to] = INF;
    return 1;
}


void GraphM::displayAllPaths(){
    int f, t, c;
    for(int i = 0; i < 100; ++i){
        for(int j = 0; j < 100; ++j){
            f = i;
            t = j;
            c = AdjM[f][t];
            if(c != 100) cout << "From: " << f << "   To: " << t << "  Cost: " << c << endl;
        }
    }
    cout << "\n Shortest from 3 to 4: " << PathM[3][4].dist << endl;
}

void GraphM::findShortestPath(){
    
    for(int source = 1; source <= size; ++source){

    int minDistance = INF;
    int v = -1;

        PathM[source][source].dist = 0; //node dist to itself is zero

        for(int i = 1; i <= size; ++i){
            
            //if not visited 
            if(!PathM[source][i].visited && AdjM[source][i] < minDistance){
                minDistance = AdjM[source][i];
                v = i;
                PathM[source][v].visited = true;
                
                //compare with each adjacent to v
                for(int w = 1; w <= size; ++w){
                    if(!PathM[source][w].visited && AdjM[v][w] != INF){

                        PathM[source][w].dist = min(PathM[source][w].dist, PathM[source][v].dist + AdjM[v][w]);
                    }
                }
            }
        }
    }
        
}

    //              ---- shortest path psuedocode ----
// for (int source = 1; source <= nodeSize; source++) {
//       PathM[source][source].dist = 0;
//       // finds the shortest distance from source to all other nodes
//       for (int i = 1; i<= nodeSize; i++) {
//           find v: not visited, shortest distance at this point
//           mark v visited 
//          for each w adjacent to v
//            if (w is not visited)
//             PathM[source][w].dist=min(PathM[source][w].dist, PathM[source][v].dist+AdjM[V][W])
//       }
//    }


