#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include <fstream>
#include <string>
#include "graph.h"
#include "context.h"

class command{
public:
    command();

    static void display_menu();
    static void display_calculus_menu();
    void display_edges();

    bool read_graph();
    void read_weights();

    void calcul_choice();
    void save_figure();
    void remove_edge_from_input();
    void ask_for_connexity();

    void display_ctr(std::vector<double> ctr);
    void compare_ctr();
    void save_centralities();
private:
    context ctx_;
    void calcul_from_id(unsigned int id, std::vector<double>& ctr, bool normalize);
};

#endif // COMMAND_H_INCLUDED
