#ifndef CENTRALITY_H_INCLUDED
#define CENTRALITY_H_INCLUDED

#include "graph.h"
#include "vertex.h"

namespace centrality{

    void betweenness_centrality_naive(graph& g, std::vector<double>& ctr, bool normalize);
    void betweenness_centrality(graph& g, std::vector<double>& ctr, bool normalize);
    void betweenness_path(graph& g, unsigned int i, unsigned int j, const std::vector<double>& dijkstra,
                            double cnt_paths, std::vector<double>& ctr);
    bool is_connex(graph& g);
    double global_centrality(std::vector<double>& ctr);

    void dijkstra(graph& g, unsigned int source, std::vector<double>& d1);
    void dijkstra_ctn_paths(graph& g, unsigned int source, std::vector<double>& d, std::vector<unsigned int>& cnt_paths);
    void bfs(graph& g, std::vector<bool>& hits, unsigned int idx=0);
}

#endif // CENTRALITY_H_INCLUDED
