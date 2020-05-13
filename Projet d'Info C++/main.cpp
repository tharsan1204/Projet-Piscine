// Projet Théorie des graphes DOS SANTOS PAVARD SRITHARAN ING2 TD03
/** Concernant les sources
- https://groupefmr.hypotheses.org/2324
- https://fr.wikipedia.org/wiki/Centralit%C3%A9
- TP2 et TP3 des TD
*/

#include <iostream>
#include <fstream>
#include <string>
#include "vertex.h"
#include "graph.h"
#include "svgfile.h"
#include "centrality.h"
#include "command.h"

using namespace std;

int main(){
    command cmd;
    command::display_menu();
    int choix;
    //On demande un fichier de graph jusqu'a temps qu'il en soit fourni un correcte.
    while(!cmd.read_graph());
    do{
        std::cout << "Choix menu : ";
        std::cin >> choix;
        switch(choix){
        case 0:
            break;
        case 1:
            cmd.read_graph();
            break;
        case 2:
            cmd.read_weights();
            break;
        case 3:
            cmd.calcul_choice();
            break;
        case 4:
            cmd.display_edges();
            break;
        case 5:
            cmd.remove_edge_from_input();
            break;
        case 6:
            cmd.ask_for_connexity();
            break;
        case 7:
            cmd.save_figure();
            break;
        case 8:
            cmd.save_centralities();
            break;
        case 9:
            cmd.compare_ctr();
            break;
        default:
            std::cout << "Anomalie choix menu" << std::endl;
            break;
        }
    }
    while ( choix!=0 );

    return 0;
}
