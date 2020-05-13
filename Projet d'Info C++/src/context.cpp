#include "context.h"

context::context(){
    std::vector<double> current_ctr_();
    std::vector<double> prev_ctr_();
}

 /**
 *  Retourne la liste de valeur de centralite courrante.
 */
 std::vector<double> context::current_ctr() const{ return current_ctr_; }

  /**
 *  Retourne la liste des anciennes valeurs de centralite.
 */
 std::vector<double> context::prev_ctr() const{ return prev_ctr_; }

 /**
*   On maintient des historiques des valeurs de centralité et limités a deux sets de valeurs
*   que l'utilisateur peut vouloir comparer.
*/
void context::push_ctr(std::vector<double> ctr){
     if(current_ctr_.empty())
        current_ctr_=ctr;
     else{
        prev_ctr_=current_ctr_;
        current_ctr_=ctr;
     }
 }

void context::push_graph(graph g){ curr_graph_=g; }
graph context::curr_graph(){ return curr_graph_; }

void context::notify_last_ope(unsigned int id){ last_ope_id_=id; }
unsigned int context::last_ope_id(){ return last_ope_id_; }

void context::set_normalisation_state(bool b){ ctr_normalized_=b;}
bool context::are_ctr_normalized(){ return ctr_normalized_; }

void context::clear_ctr(){
    current_ctr_=std::vector<double>();
    prev_ctr_==std::vector<double>();
}

