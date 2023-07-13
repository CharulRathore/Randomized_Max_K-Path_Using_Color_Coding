#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

typedef pair<int, double> EDGE;
typedef unordered_map<int, vector<EDGE>> GRAPH;

typedef unordered_map<int, unordered_map<int, double>> WEIGHTS_MAP;
typedef unordered_map<int, unordered_map<int, int>> PREV_VERTICES_MAP;

void createGraph(GRAPH & graph) {
    vector<EDGE> v1{make_pair(2, 2), make_pair(7, 10)};
    vector<EDGE> v2{make_pair(3, 8), make_pair(4, 12)};
    vector<EDGE> v3{make_pair(4, 4)};
    vector<EDGE> v7{make_pair(2, 5), make_pair(6,2)};
    vector<EDGE> v6{make_pair(3, 9)};
    graph[1] = v1;
    graph[2] = v2;
    graph[3] = v3;
    graph[7] = v7;
    graph[6] = v6;
} 

void topological_sort(GRAPH & graph, vector<int> & tvs) {
    // currently hard coded
    // TO DO:: write code to automate this
    tvs.push_back(1);
    tvs.push_back(7);
    tvs.push_back(2);
    tvs.push_back(6);
    tvs.push_back(3);
    tvs.push_back(4);
}

void initialize_weights(vector<int> & tvs, WEIGHTS_MAP & weights, PREV_VERTICES_MAP & prev_vertices, int k) {
    for (int i=0; i < (int)tvs.size()-1; i++) {
        int vertex = tvs[i];
        weights[vertex][1] = 0;
        prev_vertices[vertex][1] = -1;
        for (int h = 2; h<=k; h++) {
            weights[vertex][h] = -9999;
            prev_vertices[vertex][h] = -1;
        }
    }
}

int main() {
    int k = 4;
    int runs = 0;
    GRAPH graph;
    createGraph(graph);

    vector<int> tvs; // tvs is the vector of vertices in topologically sorted order
    topological_sort(graph, tvs);

    WEIGHTS_MAP weights;
    PREV_VERTICES_MAP prev_vertices;

    initialize_weights(tvs, weights, prev_vertices, k);

    for (int i=0; i<(int)tvs.size()-1; i++) {
        int vertex = tvs[i];
        vector<EDGE> edges = graph[vertex];
        for (vector<EDGE>::iterator eit = edges.begin(); eit != edges.end(); eit++) {
            EDGE edge = *eit;
            for (int h=1; h<k; h++) {
                int next_v = edge.first;
                double cost = edge.second;
                runs+=1;
                if (weights[vertex][h] != -9999 &&  
                         weights[vertex][h] + cost > weights[next_v][h+1]) {
                    weights[next_v][h+1] = weights[vertex][h] + cost;
                    prev_vertices[next_v][h+1] = vertex;            
                }
            }
        }
    }

    int max = -999;
    for (int i=0; i < (int)tvs.size(); i++) {
        int vertex = tvs[i];
        if (weights[vertex][k] > max) {
            max = weights[vertex][k];
        }
    }
    cout << max;
    cout << "runs: " << runs << endl;
    return 0;
} 