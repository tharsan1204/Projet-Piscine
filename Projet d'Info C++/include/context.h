#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include "graph.h"

class context{
public:
    context();

    std::vector<double> current_ctr() const;
    std::vector<double> prev_ctr() const;
    void push_ctr( std::vector<double> ctr);

    void push_graph(graph g);
    graph curr_graph();

    void notify_last_ope(unsigned int id);
    unsigned int last_ope_id();

    void set_normalisation_state(bool b);
    bool are_ctr_normalized();

    void clear_ctr();

private:
    std::vector<double> current_ctr_;
    std::vector<double> prev_ctr_;
    graph curr_graph_;
    unsigned int last_ope_id_;
    bool ctr_normalized_;
};

#endif // CONTEXT_H_INCLUDED
