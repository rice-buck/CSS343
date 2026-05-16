#include <iostream>
#include <string>
#include <fstream>

#define MAXNODES 100 //not sure how this should be defined?

using namespace std;

struct EdgeNode;      // forward reference for the compiler

struct GraphNode {    
    EdgeNode* edgeHead; // head of the list of edges
    string data;     // data information about each node 
    bool visited;  // used for DFS and BFS             
};


struct EdgeNode {
    int adjGraphNode;  // subscript of the adjacent graph node
    EdgeNode* nextEdge; // next EdgeNode
};


class GraphL {                           
   public:
   //constructor
    GraphL();

    //destructor
    ~GraphL();

    //buildGraph: builds the graph's node information and adjacency list by reading from a data file. Edge weights in the file must be ignored. Return -1 if fail. Otherwise, return 1.
    int buildGraph(ifstream& file);

    //getSize: return the "size" of the graph (# of nodes)
    int getSize();

    //displayGraph: Displays each node information and edge in the graph (e.g., the top portion of Figure 2 (c)). If fails, return -1, otherwise return 1.
    int displayGraph();

    //edgeList: returns the edge list for a given node id
    string edgeList(int nodeId);

    /*
    DFSorder: Makes a depth-first search and displays each node in depth-first order 
    starting from "start" node. The "start" is the node's integer id.  Note you must use an iterative version using stack. Recursive version is not acceptable. 
    Return the DFS in string, list node id's with space between. Note that you can call this without parameter, as it is declared with initialization.
    string DFSorder(int start=1);
    */
    string DFSorder(int start=1);

    /*
    BFSorder: Makes a breadth-first search and displays each node in depth-first order starting from "start" node. The "start" is the node's integer id.  
    Note you must use an iterative version using queue. Recursive version is not acceptable. Return the BFS in string, list node id's with space between. 
    Note that you can call this without parameter, as it is declared with initialization.
    */
    string BFSorder(int start=1);

   private:
      // array of GraphNodes
      GraphNode *nodeArray;
};