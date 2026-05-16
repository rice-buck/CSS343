#include "graphl.h"
#include <fstream>
#include <stack>
#include <string>

//constructor
GraphL::GraphL(){
    size = 0;
    
    //initalize nodeArray
    nodeArray = new GraphNode[MAXNODES];

    for(int i = 0; i < MAXNODES; ++i){
        nodeArray[i].data = "";
        nodeArray[i].edgeHead = nullptr;
        nodeArray[i].visited = false;
    }
}


//destructor
GraphL::~GraphL(){

}


int GraphL::buildGraph(ifstream& file){
    if(!file) {
        std::cerr << "Unable to open file!";
        return -1;
    }

    string line; 

    //read size
    file >> size;
    file.ignore(); // Clears the '\n' after size 

    //read node names 
    for(int i = 0; i <= size; ++i){
        getline(file, line);
        nodeArray[i].data = line; 
    }

    //read edges
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int from, to;
        
        if(ss >> from >> to){
            if(from == 0 && to == 0) break;

            EdgeNode* newEdge = new EdgeNode;
            newEdge->adjGraphNode = to;
            newEdge->nextEdge = nodeArray[from].edgeHead;
            nodeArray[from].edgeHead = newEdge;
        }
    }
    return 1;
}


int GraphL::getSize(){
    return size;
}



int GraphL::displayGraph(){
    cout << "Graph: " << endl;
    
    for(int i = 1; i <= size; ++i){
        cout << "Node " << i << "\t" << nodeArray[i].data << endl;

        EdgeNode* current = nodeArray[i].edgeHead;

        while(current != nullptr){
            cout << i << " " << current->adjGraphNode << endl;
            current = current->nextEdge;
        }
        cout << endl;
    }
    return 1; 
}

string GraphL::edgeList(int nodeId){
    string list;

    EdgeNode* current = nodeArray[nodeId].edgeHead;
    if(current == nullptr) return "No edges"; //when node has no edges going out

    while(current != nullptr){
            list += to_string(current->adjGraphNode);
            list += " ";
            current = current->nextEdge;
        }
    return list; 
}


// nodeArray[i] : Accesses the node at index i.
// nodeArray[i].edgeHead : Accesses the pointer to the first edge of node i.
// nodeArray[i].edgeHead->adjGraphNode : Accesses the target ID of that first edge.nodeArray[i].
// edgeHead->nextEdge : Steps to the second edge in the list.

//1 5 4 2 3

string GraphL::DFSorder(int start){
    //reset visited to be abel to run DFS multiple times
    for(int i = 1; i <= size; ++i) {
        nodeArray[i].visited = false;
    }

    stack<int> s;
    string dfs = "";

    s.push(start); //push starting node onto stack

    while(!s.empty()){
        //pop top node from stack
        int topNode = s.top(); 
        s.pop();

        if(!nodeArray[topNode].visited){
            //mark as visited
            nodeArray[topNode].visited = true;
            dfs += to_string(topNode) + " "; //add to dfs order
        }

        //create temp stack for reversing neighbor order
        stack<int> tempS;
        EdgeNode* current = nodeArray[topNode].edgeHead;

        //put neighbors into temp stack to reverse them once
        while(current != nullptr){
            if(!nodeArray[current->adjGraphNode].visited){
                tempS.push(current->adjGraphNode);
            }
            current = current->nextEdge;
        }

        //unload temp stack into main stack
        while(!tempS.empty()){
            s.push(tempS.top());
            tempS.pop();
        }

    }

    return dfs;
}