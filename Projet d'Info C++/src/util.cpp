#include "util.h"
/**
* Normalise la valeur d'un point entre 0 et 1 compte tenu des valeurs minimale et maximale.
*/
double util::normalize_data(double value, double max, double min){
    return (value-min)/(max-min);
}
