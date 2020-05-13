#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <string>
#include <vector>

class vertex{
public :
    vertex(std::string name, int x, int y);
    vertex();
    void add_neighbour(int idx);
    int get_x();
    int get_y();
    std::string get_name();

private :
    int index_;
    std::string name_;
    int x_;
    int y_;
};

#endif // VERTEX_H_INCLUDED
