#include "graphm.h"
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>

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
    cout << "Description     \tFrom_node\tTo_node \tDistance\tPath\n" << endl;
    for (int source = 1; source <= size; ++source){
        cout << vertices[source] << endl;
        for(int to = 1; to <= size; ++to){
            if(source != to){
                string dist;
                if(PathM[source][to].dist != INF) dist = to_string(PathM[source][to].dist);
                else dist = "---";

                cout << "\t\t\t" << source << "\t\t" << to << "\t\t" << dist << "\t\t";

                //print path
                if(dist != "---"){
                    stack<int> prevNode;
                    int toPath = to;
                    
                    while(PathM[source][toPath].prev_node != 0){
                        prevNode.push(PathM[source][toPath].prev_node);
                        toPath = PathM[source][toPath].prev_node;
                    }
                    
                    //print path from stack
                    while (!prevNode.empty()) {
                        cout << prevNode.top() << " ";
                        prevNode.pop();
                    }
                    cout << to << endl; 
                    
                } else cout << endl;
            }
        }
    }
}




void GraphM::findShortestPath(){
    for(int source = 1; source <= size; ++source){
        PathM[source][source].dist = 0; //distance to itself is zero

        for(int i = 1; i <= size; ++i){

            int vDist = INF;
            int v = -1; 

            //find shortest from source to all other nodes (find v)
            for(int findV = 1; findV <= size; ++findV){
                if(!PathM[source][findV].visited){ //find shortest dist neighbor of source
                    if(PathM[source][findV].dist < vDist){
                        vDist = PathM[source][findV].dist;
                        v = findV;
                    }
                }
            }

            if (v == -1) break; //if a node is disconnected, then v will remain -1

            //mark v visited
            PathM[source][v].visited = true;
            PathM[source][v].dist = vDist;

            //explore each w adjacent to v
            for(int w = 1; w <= size; ++w){
                if(!PathM[source][w].visited && AdjM[v][w] != INF){
                    if(PathM[source][v].dist + AdjM[v][w] < PathM[source][w].dist){
                        PathM[source][w].dist = PathM[source][v].dist + AdjM[v][w];
                        PathM[source][w].prev_node = v;
                    }
                }
            }
        }
    }
}
