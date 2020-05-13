#include "graph.h"
#include "svgfile.h"
#include "centrality.h"
#include <iostream>
#include <math.h>
#include <algorithm>

#define LONG_MAX 2147483647.0

graph::graph(std::vector<vertex>& vertices, bool directed) : directed_{directed}{
    std::vector<std::pair<unsigned int,unsigned int>&> edges_();
    std::vector<double> edge_weights_();
    vertices_ = vertices;
    nb_vertices_=vertices_.size();

    dijkstra_=std::vector<std::vector<double>>(nb_vertices_);
    nb_paths_=std::vector<std::vector<unsigned int>>(nb_vertices_);
}

graph::graph(){};

graph::~graph() = default;

void graph::set_edge_list(std::vector<std::pair<unsigned int,unsigned int>>& edges){
    edges_=edges;
    edge_weights_ = std::vector<double>(edges.size(), 1.0);
    refresh_dijkstra_map();
}
bool graph::set_edge_weights(std::vector<double>& edge_weights){
    if(edge_weights.size()!=edges_.size())
        return false;
    edge_weights_ = edge_weights;
    refresh_dijkstra_map();
    return true;
}

std::vector<std::pair<unsigned int,unsigned int>> graph::get_edges() const{ return edges_; }
std::vector<vertex> graph::get_vertices() const{ return vertices_; }
unsigned int graph::get_nb_vertices() const{ return nb_vertices_;}

std::vector<std::vector<double>> graph::get_dijkstra_map() const{ return dijkstra_; }
std::vector<std::vector<unsigned int>> graph::get_nb_path_map() const{ return nb_paths_; }

void graph::refresh_dijkstra_map(){
    for(unsigned int i=0; i<nb_vertices_; ++i)
        centrality::dijkstra_ctn_paths(*this, i, dijkstra_[i], nb_paths_[i]);
}

bool graph::is_directed() const{ return directed_; }

bool graph::remove_edge(unsigned int idx){
    if(idx>edges_.size())
        return false;
    edges_.erase(edges_.begin()+idx);
    return true;
}

/**
*   Retourne la liste des voisins du sommet idx
*/
std::vector<unsigned int> graph::get_neighbours(unsigned int idx){
    std::vector<unsigned int> n;
    for(unsigned int i=0; i<edges_.size(); ++i)
        if(edges_[i].first==idx)
            n.push_back(edges_[i].second);
        else if((!directed_ && edges_[i].second==idx))
            n.push_back(edges_[i].first);
    return n;
}

/**
*   Retourne la liste es predecesseurs du sommet idx.
*/
std::vector<unsigned int> graph::get_pred(unsigned int idx){
    if(!directed_)
        return get_neighbours(idx);

    std::vector<unsigned int> n;
    for(unsigned int i=0; i<edges_.size(); ++i)
        if(edges_[i].second==idx)
            n.push_back(edges_[i].first);
    return n;
}

double graph::get_weight(std::pair<unsigned int, unsigned int> edge){
    for(unsigned int i=0; i<edges_.size(); ++i)
        if(edges_[i]==edge)
            return edge_weights_[i];
        else if(!directed_ && std::pair<unsigned int, unsigned int>(edge.second, edge.first)==edges_[i])
            return edge_weights_[i];
    std::cout<<"Error => "<<"("<<edge.first<<","<<edge.second<<")"<<std::endl;
    return -1;
}
