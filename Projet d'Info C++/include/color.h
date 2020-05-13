#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include <string>

namespace color{
    std::string makeRGB(int r, int g, int b);
    std::string get_hue_rgb(double percent);
}
