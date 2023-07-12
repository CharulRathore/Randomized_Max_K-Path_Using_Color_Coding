#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

typedef pair<int, double> EDGE;
typedef unordered_map<int, vector<EDGE>> GRAPH;
typedef unordered_map<int, char> VCOLOR;
typedef struct triplet {
    unordered_set<char> color_subset;
    double cost;
    int prev_vertex;
} scp_triplet;

typedef unordered_map<int, unordered_map<int, vector<scp_triplet>>> VTRIPLETS;

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

void colorGraph(VCOLOR & vcolor) {
    vcolor[1] = 'R';
    vcolor[2] = 'B';
    vcolor[7] = 'G';
    vcolor[6] = 'B';
    vcolor[3] = 'G';
    vcolor[4] = 'V';
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

void initialize_vertex_triplets(VTRIPLETS & vertex_triplets, vector<int> & tvs, VCOLOR & vcolor) {
    for (int i=0; i < (int)tvs.size(); i++) {
        int vertex = tvs[i];
        scp_triplet scpt;
        scpt.color_subset.insert(vcolor[vertex]);
        scpt.cost = 0;
        scpt.prev_vertex = -1;
        vertex_triplets[vertex][1].push_back(scpt);
    }
}

bool check_is_same(unordered_set<char> & cs1, unordered_set<char> & cs2) {
    for (unordered_set<char>::iterator it = cs1.begin(); it != cs1.end(); it++) {
        if (cs2.find(*it) == cs2.end()) {
            return false;
        }
    }
    return true;
}

bool is_color_subset_already_present(scp_triplet & vtrip, int next_v, VCOLOR & vcolor, 
                            VTRIPLETS & vertex_triplets, int & matching_triplet_idx) {
    unordered_set<char> temp_color_subset = vtrip.color_subset;
    temp_color_subset.insert(vcolor[next_v]);
    int csize = temp_color_subset.size();
    vector<scp_triplet> vscp = vertex_triplets[next_v][csize];
    for (int i = 0; i < (int)vscp.size(); i++) {
        scp_triplet scp = vscp[i];
        if (check_is_same(scp.color_subset, temp_color_subset)) {
            matching_triplet_idx = i;
            return true;
        }
    }
    return false;
}

int main() {
    int k = 4;
    GRAPH graph;
    VCOLOR vcolor;
    createGraph(graph);
    colorGraph(vcolor);

    vector<int> tvs; // tvs is the vector of vertices in topologically sorted order
    topological_sort(graph, tvs);

    // for each vertex, we will keep a vector of triplets which will define the cost of 
    // path P for each 1<=h<=k. For a h-path P, there can be multiple triplets with diff.
    // cost and diff. subset of colors. We need to store each of them.
    VTRIPLETS vertex_triplets;

    // initialize all vertex triplets for 1-path P i.e. P1 as 
    // ({C[v]}, 0, *) -> C[v] is the color of that vertex from the colored graph
    // the cost of P1 for that vertex is 0 as P1 mean is it is the starting vertex.
    // Since there is no prev vertex for P1, keep the 3rd element of triplet as *
    initialize_vertex_triplets(vertex_triplets, tvs, vcolor);

    for (int h = 1; h < k; h++) {   // h represents path P with h vertices
        for (int i=0; i < (int)tvs.size(); i++) {
            int vertex = tvs[i];
            if (graph.find(vertex) == graph.end()) {
                continue;
            }
            vector<EDGE> edges = graph[vertex];
            for (vector<EDGE>::iterator eit = edges.begin(); eit != edges.end(); eit++) {
                EDGE edge = *eit;
                int next_v = edge.first;
                double e_weight = edge.second;
                vector<scp_triplet> vscps = vertex_triplets[vertex][h];
                for (vector<scp_triplet>::iterator sit = vscps.begin(); sit != vscps.end(); sit++) {
                    if ((int)((*sit).color_subset.size()) != h) {
                        // only consider triplets with color subset size = h no. of vertices
                        continue;
                    }
                    if ((*sit).color_subset.find(vcolor[next_v]) != (*sit).color_subset.end()) {
                        // only consider the color subsets which does not have color of next vertex
                        continue;
                    }
                    // Here, we need to check 2 things:
                    // 1) if S U {C(next_v)} is present in scp_triplets of next_vertex
                    // if not present then insert it in triplets of next_v
                    // else check if the cost of already present trip is less than the 
                    // new triplet
                    scp_triplet curr_trip = *sit;
                    int  matching_triplet_idx;
                    if (!is_color_subset_already_present(curr_trip, next_v, 
                                                    vcolor, vertex_triplets, 
                                                    matching_triplet_idx)) {
                        scp_triplet new_trip;
                        new_trip.color_subset = curr_trip.color_subset;
                        new_trip.color_subset.insert(vcolor[next_v]);
                        new_trip.cost = curr_trip.cost + e_weight;
                        new_trip.prev_vertex = vertex;
                        vertex_triplets[next_v][h+1].push_back(new_trip);
                    } else {
                        if (vertex_triplets[next_v][h+1][matching_triplet_idx].cost < curr_trip.cost + e_weight) {
                            vertex_triplets[next_v][h+1][matching_triplet_idx].cost = curr_trip.cost + e_weight;
                            vertex_triplets[next_v][h+1][matching_triplet_idx].prev_vertex = vertex;        
                        }
                    }
                }
            }
        }
    }
    int pmax = -999;
    for (int v = 0; v < (int)tvs.size(); v++) {
        int vertex = tvs[v];
        vector<scp_triplet> vscpt = vertex_triplets[vertex][k];
        for (vector<scp_triplet>::iterator vit = vscpt.begin(); vit != vscpt.end(); vit++) {
            scp_triplet scp = *vit;
            if (scp.cost > pmax) {
                pmax = scp.cost;
            }
        }
    }
    cout << pmax;
    return 0;
}