#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>
#include <iostream>
#include "graph.h"

using namespace std;

int main() {

    string inputFile = "./graph_dataset/graph_with_7V_9E";
    Graph g(inputFile);

    if(!g.createGraph()) {
        cout << "Graph not created!!" << endl;
        return 0;
    } 

    GRAPH graph = g.getGraph();

    
    


}