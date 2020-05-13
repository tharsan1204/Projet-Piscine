#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <vector>
#include <map>
#include "vertex.h"

class graph {
public:
    graph(std::vector<vertex>& vertices, bool directed);
    graph();
    ~graph();
    void set_edge_list(std::vector<std::pair<unsigned int,unsigned int>>& edges);
    bool set_edge_weights(std::vector<double>& edge_weights);

    std::vector<std::pair<unsigned int,unsigned int>> get_edges() const;
    std::vector<vertex> get_vertices() const;
    unsigned int get_nb_vertices() const;
    std::vector<std::vector<double>> get_dijkstra_map() const;
    std::vector<std::vector<unsigned int>> get_nb_path_map() const;

    bool is_directed() const;
    bool remove_edge(unsigned int idx);

    std::vector<unsigned int> get_neighbours(unsigned int idx);
    std::vector<unsigned int> get_pred(unsigned int idx);
    double get_weight(std::pair<unsigned int, unsigned int> edge);

private:
    bool directed_;
    unsigned int nb_vertices_;

    std::vector<std::pair<unsigned int,unsigned int>> edges_;
    std::vector<double> edge_weights_;
    std::vector<vertex> vertices_;

    std::vector<std::vector<double>> dijkstra_;
    std::vector<std::vector<unsigned int>> nb_paths_;

    void refresh_dijkstra_map();
};

#endif // GRAPH_H_INCLUDED
