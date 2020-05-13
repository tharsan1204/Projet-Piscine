#include "vertex.h"

vertex::vertex( std::string name, int x, int y) : name_{name}, x_{x}, y_{y}{}
vertex::vertex(){};
int vertex::get_x(){ return x_; }
int vertex::get_y(){ return y_; }
std::string vertex::get_name(){ return name_; }
