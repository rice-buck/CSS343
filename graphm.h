#include <iostream>
#include <string>
#include <fstream>

#define MAXNODES 4 //not sure how this should be defined?


using namespace std;

struct TableType {
         bool visited;          // whether node has been visited
         int dist;              // shortest distance from source known so far
         int prev_node;              // previous node in path of min dist
};

class GraphM {                           
   public:
    // constructor: Initialize private data members. All entries of AdjM set to infinity.  
    // All entires of PathM is initialized to sets all dist to infinity, 
    // sets all visited to false, and sets all prev_node to 0.
    GraphM(); 


    //buildGraph: Reads node descriptions and adjacency data from a file, building the graph. Return -1 if fail. Otherwise, return 1. 
    int buildGraph(ifstream& file);


    //InsertEdge: inserts an edge into the adjacency matrix by providing the weight between two given nodes. Return -1 if fail. Otherwise, return 1.
    int insertEdge(int from, int to, int cost);


    //removeEdge: removes an edge by resetting its weight to the default "infinity" value. Return -1 if fail. Otherwise, return 1.
    int removeEdge(int from, int to);


    //findShortestPath: computes the shortest paths between all pairs of nodes using Dijicstra's shortest path algorithm. PathM should be updated with shortest path information.  
    void findShortestPath();


    //displayAllPaths: outputs all shortest-path distances and paths using cout
    void displayAllPaths();

    
   private:
      string vertices[MAXNODES];              // data for graph nodes
      int AdjM[MAXNODES][MAXNODES];            // adjacency matrix 
      int size;                             // number of nodes in the graph
      TableType PathM[MAXNODES][MAXNODES];      // distance matrix: stores visited, distance, path
};



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