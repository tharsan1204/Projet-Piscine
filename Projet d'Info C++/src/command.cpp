#include "command.h"

#include "vertex.h"
#include "svgfile.h"
#include "centrality.h"
#include <fstream>
#include <string>

command::command(){
    ctx_=context();
}

void command::display_menu(){
    std::cout << "0/ Quitter" << std::endl
              << "1. Charger graph" << std::endl
              << "2. Charger poids" << std::endl
              << "3. Calculs..." << std::endl
              << "4. Lister arretes" << std::endl
              << "5. Supprimer arretes" << std::endl
              << "6. Tester connexite" << std::endl
              << "7. Afficher" << std::endl
              << "8. Sauvegarder" << std::endl
              << "9. Comparer les centralites avec les precedentes" << std::endl << std::endl;
}

void command::display_calculus_menu(){
    std::cout << "\t0/ Quitter" << std::endl
              << "\t1. Centralite de degree" << std::endl
              << "\t2. Centralite de vecteur propre " << std::endl
              << "\t3. Centralite de proximite" << std::endl
              << "\t4. Centralite d'intermediarite" << std::endl
              << "\t5. Centralite d'intermediarite simple" << std::endl
              << "\t6. Dijkstra" << std::endl << std::endl;
}

void command::display_edges(){
    graph g=ctx_.curr_graph();
    std::vector<std::pair<unsigned int,unsigned int>> e = g.get_edges();
    std::vector<vertex> v = g.get_vertices();
    std::string edge_sign;

    if(g.is_directed()) edge_sign = " -> ";
    else edge_sign = " - ";

    std::cout << "Liste des arretes:" << std::endl;
    for(unsigned int i=0; i<e.size(); ++i)
        std::cout << "\t" << i << ". " << v[e[i].first].get_name() << edge_sign << v[e[i].second].get_name() << std::endl;
}

bool command::read_graph(){
    std::string filename;
    std::cout << "Nom du fichier de graph : ";
    std::cin >> filename;
    std::ifstream fin(filename);
    //std::ifstream fin("res/test_topo.txt");
    if (!fin.is_open()){
        std::cout<<"Impossible d'ouvrir le fichier de sortie."<<std::endl;
        return false;
    }


    graph g;
    bool directed;
	fin >> directed;
	int num_vertices, num_edges, idx, x, y, t, s;
	std::string name;

	fin >> num_vertices;
	std::vector<vertex> vertices(num_vertices);
	for (int i = 0; i < num_vertices; ++i){
        fin >> idx;
        fin >> name;
        fin >> x;
        fin >> y;
		vertices[idx] = vertex(name, x, y);
	}

	g = graph(vertices, directed);

	fin >> num_edges;
	std::vector<std::pair<unsigned int,unsigned int>> edges(num_edges);
	for(int i = 0; i<num_edges; ++i){
        fin >> idx;
        fin >> s;
        fin >> t;
        edges[idx]={s, t};
	}
	g.set_edge_list(edges);

    ctx_.push_graph(g);

    std::cout<<"OK."<<std::endl;
    fin.close();
    ctx_.clear_ctr();
    return true;
}

void command::read_weights(){
    std::string filename;
    std::cout << "Nom du fichier de poids : ";
    std::cin >> filename;
    std::ifstream fin(filename);
    //std::ifstream fin("res/w_test_topo.txt");
    if (!fin.is_open()) std::cout<<0;

    int num_weights, idx, w;

	fin >> num_weights;
	std::vector<double> weights (num_weights);
	for (int i = 0; i < num_weights; ++i){
        fin >> idx;
        fin >> w;
		weights[idx] = w;
	}
	graph g = ctx_.curr_graph();

	if(!g.set_edge_weights(weights))
    std::cout << "Erreur: Nombre de poid different du nombre d'arretes." << std::endl;
    else{
        ctx_.push_graph(g);
        std::cout<<"OK."<<std::endl;
    }
}

void command::calcul_choice(){
    int choix_calcul;
    bool normalize;
    std::vector<double> ctr;
    display_calculus_menu();

    std::cout << "\tChoix calcul : ";
    std::cin >> choix_calcul;
    if(choix_calcul!=2&&choix_calcul!=6&&choix_calcul!=0){
        std::cout << "\tNormaliser les indices ? (0:Non, 1:Oui) : ";
        std::cin >> normalize;
    }

    if(choix_calcul!=0){
        ctx_.notify_last_ope(choix_calcul);
        ctx_.set_normalisation_state(normalize);
        calcul_from_id(choix_calcul, ctr, normalize);

        ctx_.push_ctr(ctr);

        std::cout << "| ";
        for(auto e:ctx_.current_ctr())
        std::cout << e << " | " ;
        std::cout << std::endl << std::endl;
    }
}

void command::calcul_from_id(unsigned int id, std::vector<double>& ctr, bool normalize){
    graph g=ctx_.curr_graph();
    switch(id){
        case 0:
            break;
        case 1:
            centrality::degree(g, ctr, normalize);
            break;
        case 2:
            centrality::eigenvector_centrality(g, ctr);
            break;
        case 3:
            centrality::closeness_centrality(g, ctr, normalize);
            break;
        case 4:
            centrality::betweenness_centrality(g, ctr, normalize);
            break;
        case 5:
            centrality::betweenness_centrality_naive(g, ctr, normalize);
            break;
        case 6:
            centrality::dijkstra(g, 0, ctr);
    }
}

void command::save_figure(){
    if(ctx_.current_ctr().empty())
        Svgfile::draw_graph(ctx_.curr_graph(),"out/degree.svg");
    else
        Svgfile::draw_graph(ctx_.curr_graph(),"out/degree.svg", ctx_.current_ctr());
}

void command::remove_edge_from_input(){
    graph g=ctx_.curr_graph();
    unsigned int idx;
    std::cout << "Index de l'arrete a supprimer : ";
    std::cin >> idx;
    if(g.remove_edge(idx)){
        ctx_.push_graph(g);
        ask_for_connexity();

        std::vector<double> ctr;
        calcul_from_id(ctx_.last_ope_id(), ctr, ctx_.are_ctr_normalized());
        ctx_.push_ctr(ctr);

        compare_ctr();

    }else std::cout << "Impossile de supprimer cette arrête." << std::endl;
}

void command::ask_for_connexity(){
    graph g=ctx_.curr_graph();
    std::cout<<"Connexe ? => ";
    if(centrality::is_connex(g))
        std::cout<<"Oui\n";
    else
        std::cout<<"Non\n";
}

void command::display_ctr(std::vector<double> ctr){
    std::cout << "| ";
    for(auto e:ctr)
    std::cout << e << " | " ;
    std::cout << std::endl << std::endl;
}
void command::compare_ctr(){
    std::cout << "Anciennes valeurs de centralite" << std::endl;
    display_ctr(ctx_.prev_ctr());

    std::cout << "Nouvelles valeurs de centralite" << std::endl;
    display_ctr(ctx_.current_ctr());
}

void command::save_centralities(){
    std::ofstream fout("out/scores_centralite.txt");
    if(!fout.is_open()){
        std::cout<<"Impossible d'ouvrir le fichier de sortie."<<std::endl;
        return;
    }
    std::vector<std::vector<double>> ctrs(9);
    unsigned int id=1;
    for(int i=0; i<9; ++i){
        calcul_from_id(id, ctrs[i], false);
        if(id!=2){
            ++i;
            calcul_from_id(id, ctrs[i], true);
        }
        ++id;
    }

    for(int i=0; i<(int)ctx_.curr_graph().get_nb_vertices(); ++i){
        fout << i << " ";
        for(int j=0; j<9; ++j){
            fout << ctrs[j][i] << " ";

        }
        fout << std::endl;
    }

    fout.close();
    std::cout << "Sauve dans out/scores_centralite.txt." << std::endl;
}
