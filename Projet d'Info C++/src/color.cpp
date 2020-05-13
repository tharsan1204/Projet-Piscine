#include "color.h"
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

namespace color{
    std::string makeRGB(int r, int g, int b){
        std::ostringstream oss;
        oss << "rgb(" << r << "," << g << "," << b << ")";
        return oss.str();
    }

    /**
    *   Retourne une string correspondant à la couleur format rgb
    *   déduite du pourcentage passé en paramètre.
    */
    std::string get_hue_rgb(double percent){
        int d=(1-percent)*120;
        double hue;

        hue=1-std::abs(std::fmod((d/60.0),2.0) - 1.0); // fmod permet

        if(d < 60)
            return makeRGB(255, hue*255, 0);
        else
            return makeRGB(hue*255, 255, 0);
    }
}
