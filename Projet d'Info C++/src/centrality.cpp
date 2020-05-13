#include "centrality.h"
#include <math.h>
#include <queue>
#include <iostream>

#define LONG_MAX 2147483647.0

namespace centrality{

    /**
    *   Calcul du degree du sommet 'idx'.
    */
    double degree(graph& g, unsigned int idx, bool normalize){
        std::vector<std::pair<unsigned int, unsigned int>> edges=g.get_edges();

        double d = 0;
        for(unsigned int i = 0; i<edges.size(); ++i)
            if(edges[i].first==idx || edges[i].second==idx)
                d++;
        return normalize ? d/(g.get_nb_vertices()-1) : d;
    }

    /**
    *   Calcul du degree de tous les sommets du graph 'g'.
    */
    void degree(graph& g, std::vector<double>& ctr, bool normalize){
        ctr = std::vector<double>(g.get_nb_vertices());
        for(unsigned int i=0; i<g.get_nb_vertices(); ++i)
            ctr[i] = degree(g, i, normalize);
    }

    /**
    *   Calcule les valeurs de centralité de vecteur propre et les stocke dans 'str'.
    */
    void eigenvector_centrality(graph& g, std::vector<double>& ctr){
        std::vector<std::pair<unsigned int, unsigned int>> edges=g.get_edges();
        // l: lambda, old_l: valeur de 'l' à l'iteration pecedente.
        // d: variation de l entre deux iterations.
        double l=1.0, old_l=0.0, d=1.0, n=g.get_nb_vertices();
        bool directed = g.is_directed();
        ctr=std::vector<double>(n, 1.0);

        // Tant que la valeur de 'l' varie trop.
        while(d>0.01){
            //Pour chaque sommet 𝑠𝑖, faire la somme des indices des ses voisins
            std::vector<double> c(n, 0.0);
            for(unsigned int i=0; i<n; ++i){
                for(auto e:edges)
                    if(e.first==i)
                        c[i]+=ctr[e.second];
                    else if(!directed && e.second==i)
                        c[i]+=ctr[e.first];
            }
            //Calculer lambda
            old_l=l;
            l=0.0;
            for(auto i: c){
                l+=i*i;
            }
            l = sqrt(l);

            //Pour chaque sommet, recalculer l’indice
            for(unsigned int i=0; i<n; ++i){
                ctr[i]=c[i]/l;
            }
            d=abs(old_l-l);
        }
    }

    /**
    *   Calcule les valeurs de centralité de proximité et les stocke dans 'str'.
    */
    void closeness_centrality(graph& g, std::vector<double>& ctr, bool normalize){
        unsigned int n = g.get_nb_vertices();
        double tmp;
        ctr = std::vector<double>(n, 0.0);
        // Liste des listes des valeurs de plus courts chemins à partir de chasue sommet
        // d[O] retourne un vector<double> contenant les longueurs des plus courts chemins
        // du sommet à chaque autre sommet.
        std::vector<std::vector<double>> d=g.get_dijkstra_map();
        std::vector<double> d_i;

        // Pour chaque sommet j on stocke dans tmp la somme des longueurs des plus courts
        // chemins de j à k
        for(unsigned int i=0; i<n; ++i){
            d_i=d[i];

            tmp=0.0;
            for(unsigned int j=0; j<n; ++j)
                tmp+=d_i[j];
            ctr[i]=1/tmp;
            if(normalize)
                ctr[i]*=n-1.0;
        }
    }

    /**
    *   Calcul de la centralite intermediaire qui ne prend en compte
    *   qu'un seul des possibles plus court chemins de j a k.
    */
    void betweenness_centrality_naive(graph& g, std::vector<double>& ctr, bool normalize){
        int n=g.get_nb_vertices(), i, tmp_i, w, min, pred;
        std::vector<std::vector<double>> d=g.get_dijkstra_map();
        std::vector<double> d_i;
        ctr = std::vector<double>(n, 0.0);

        for(int j=0; j<n; ++j){
            d_i=d[j];
            for(int k=j+1;   k<n; ++k){
                i=k;
                // Tant qu'on est pas remonté au point d'origine j on incrémente
                // la valeur du predecesseur p de i avec la plus petite valeur de plus court chemin de
                // j à p.
                while(d_i[i]!=0&&d_i[i]!=LONG_MAX){
                    auto preds=g.get_pred(i);
                    min=LONG_MAX;

                    for(unsigned int p=0; p<preds.size(); ++p){
                        pred=preds[p];
                        w=g.get_weight({pred, i});
                        if(d_i[pred]+w<min){
                            min=d_i[pred]+w;
                            tmp_i=pred;
                        }
                    }
                    i=tmp_i;
                    if(d_i[i]==0||d_i[i]==LONG_MAX)
                        break;
                    ctr[i]++;
                }
            }
        }
        if(normalize)
            for(int i=0; i<n; ++i)
                ctr[i]*=(2.0/(double)(n*n-3*n+2));
    }

    /**
    *   Calcul de la centralite intermediaire prennant en compte
    *    tous les plus court chemins de j a k.
    */
    void betweenness_centrality(graph& g, std::vector<double>& ctr, bool normalize){
        unsigned int n=g.get_nb_vertices();
        std::vector<std::vector<double>> d=g.get_dijkstra_map();
        std::vector<double> ctr_k, d_j;
        ctr=std::vector<double>(n, 0.0);
        std::vector<std::vector<unsigned int>> cnt_paths=g.get_nb_path_map();
        std::vector<unsigned int> cnt_paths_j;

        // Pour tous les sommets on calcule les centralités (pas encore tout a fait calculées)
        // sur le chemin de j à k dans ctr_k.
        for(unsigned int j=0; j<n; ++j){
            d_j=d[j];

            for(unsigned int k=j+1; k<n; ++k){
                betweenness_path(g, k, j, d_j, cnt_paths[j][k], ctr_k);

                for(unsigned int i=0; i<n; ++i)
                    ctr[i]+=ctr_k[i]*(double)cnt_paths[j][i];
            }
        }

        if(normalize)
        for(unsigned int i=0; i<n; ++i)
            ctr[i]*=(2.0/(double)(n*n-3*n+2));
    }

     /**
    * Calcul de centralite intermediaire pour tous les plus courts
    * chemins de i à j.
    */
    void betweenness_path(graph& g, unsigned int i, unsigned int j, const std::vector<double>& dijkstra,
                            double cnt_paths, std::vector<double>& ctr){
        unsigned int n=dijkstra.size(), min, pred;
        int w;
        ctr=std::vector<double>(n, 0.0);
        std::vector<double> ctr_p;
        std::vector<unsigned int> paths;
        auto preds=g.get_pred(i);
        min=LONG_MAX;

        // Pour chaque voisins de i, on ajoute à la liste des predecesseur
        // sur le plus court chemin de j au sommet final inital (paths) tous les sommets
        // avec la plus petite valeur de plus court chemin.
        for(unsigned int p=0; p<preds.size(); ++p){
            pred=preds[p];
            w=g.get_weight({pred, i});
            if(dijkstra[pred]+w<min){
                paths.clear();
                paths.push_back(pred);
                min=dijkstra[pred]+w;
            }
            else if(dijkstra[pred]+w==min){
                paths.push_back(pred);
            }
        }
        for(auto p: paths){
            if(dijkstra[p]!=0&&dijkstra[p]!=LONG_MAX){
                // Si on a pas atteint le point d'origine du chemin on ajoute à la
                // valeur d'un chemin du sommet courrant 1 divisé par le nombre de chemin allant du
                // sommet d'origine au sommet final initial : cnt_paths.
                ctr[p]+=(1/(double)cnt_paths);
                betweenness_path(g, p, j, dijkstra, cnt_paths, ctr_p);
                for(unsigned int k=0; k<n; ++k){
                    ctr[k]+=ctr_p[k];
                }
            }
        }
    }

    /**
    *   Retourne 'true' si le graph est connexe, 'false' sinon.
    */
    bool is_connex(graph& g){
        std::vector<bool> hits;
        unsigned int n;
        // Si le graph est orienté on doit tester s'il existe des chemins entre chaque sommet i et j.
        // S'il ne l'est pas, un seul sommet de départ suffit.
        if(!g.is_directed()) n=1; else n=g.get_nb_vertices();
        for(unsigned int i=0; i<n; ++i){
            bfs(g, hits, i);
            // On parcours la liste retournée par bfs, si un sommet n'ai pas été
            // touché alors on retourne false.
            for(bool c:hits)
                if(!c)
                    return false;
        }
        return true;
    }

    /**
    *   Implementation de l'algorithme 'Breadth First Search'. 'hits' est une liste
    *   de boolean avec true=>  le sommet de meme indice appartient à la meme composante connexe que le sommet 'idx'
    *   false sinon.
    */
    void bfs(graph& g, std::vector<bool>& hits, unsigned int idx){
        hits=std::vector<bool>(g.get_nb_vertices(), 0);
        std::queue<unsigned int> q;
        // on ajoute de sommet 'idx' à la liste des sommets à explorer
        q.push(idx);
        // on note le sommet 'idx' comme explore
        hits[idx]=1;

        // Tant qu'il n'y a pas de sommet a explorer
        while(!q.empty()){
            std::vector<unsigned int> n=g.get_neighbours(q.front());
            q.pop();
            for(auto e:n){
                // si le voisin 'e' n'a pas ete explore...
                if(!hits[e]){
                    // on ajoute de sommet 'e' à la liste des sommets à explorer
                    q.push(e);
                    // on note le sommet 'e' comme explore
                    hits[e]=1;
                }
            }
        }
    }

    /**
    *  'd' est la liste des longueurs du/des pluscourts chemins du sommet 'source'
    *   à chaque sommet.
    */
    void dijkstra(graph& g, unsigned int source, std::vector<double>& d){
        int n=g.get_nb_vertices(), k=1, x=source, y, weight, min;
        d=std::vector<double>(n, LONG_MAX);
        std::vector<bool> s(n,false);

        d[x]=0.0;
        s[x]=true;
        while(k<n && d[x]<LONG_MAX){
            auto neighbours=g.get_neighbours(x);
            for(unsigned int i=0; i<neighbours.size(); ++i){
                y=neighbours[i];
                if(!s[y]){
                    weight=g.get_weight({x, y});
                    if(weight<0)
                        d[y]=LONG_MAX;
                    else
                        d[y]=std::min(d[y], d[x]+weight);
                }
            }
            k++;
            min=LONG_MAX;
            for(int i=0; i<n; ++i){
                if((d[i]<min)&&!s[i]){
                    x=i;
                    min=d[i];
                }
            }
            s[x]=true;
        }
    }

    /**
    *  'd' est la liste des longueurs du/des pluscourts chemins du sommet 'source'
    *   à chaque sommet. 'cnt_paths' est le nombre de plus courts chemins de 'source'
    *   à chaque sommet.
    */
    void dijkstra_ctn_paths(graph& g, unsigned int source, std::vector<double>& d, std::vector<unsigned int>& cnt_paths){
        int n=g.get_nb_vertices(), k=1, x=source, y, weight, min;
        d = std::vector<double>(n, LONG_MAX);
        cnt_paths = std::vector<unsigned int>(n, 0);
        std::vector<bool> s(n,false);

        d[x]=0.0;
        cnt_paths[x]=1;
        s[x]=true;
        while(k<n && d[x]<LONG_MAX){
            auto neighbours=g.get_neighbours(x);
            for(unsigned int i=0; i<neighbours.size(); ++i){
                y=neighbours[i];
                if(!s[y]){
                    weight=g.get_weight({x, y});
                    if(weight>=0){
                        if(d[y] > d[x]+weight){
                            d[y]=d[x]+weight;
                            cnt_paths[y] = cnt_paths[x];
                        }
                        // Si un sommet à deja été atteint avec un chemin de même longueur
                        // alors c'est qu'il y a deux chemins jusqu'a ce sommet.
                        else if(d[y] == d[x]+weight)
                            cnt_paths[y] += cnt_paths[x];
                    }
                }
            }
            k++;
            min=LONG_MAX;
            for(int i=0; i<n; ++i){
                if((d[i]<min)&&!s[i]){
                    x=i;
                    min=d[i];
                }
            }
            s[x]=true;
        }
    }

    /**
    *   Retourne la centralite globale du reseau pour des valeurs normalisées.
    */
    double global_centrality(std::vector<double>& ctr){
        //unsigned int n=ctr.size();
        double max=0.0, g_ctr=0.0;
        for(double c:ctr)
            max=std::max(c, max);
        for(double c:ctr)
            g_ctr+=max-c;

        return g_ctr;
    }
}
