//McCAD
#include "EdgesCombiner.hpp"

void
McCAD::Decomposition::EdgesCombiner::operator()(
        std::vector<std::shared_ptr<McCAD::Geometry::Edge>>& edgesList){
    if(edgesList.size() < 2) return;
    for (int i = 0; i < edgesList.size() - 1; ++i){
        for (int j = i+1; j < edgesList.size(); ++j){
            if (*(edgesList[i]) == *(edgesList[j])){
                edgesList.erase(edgesList.begin() + j);
                --j;
            }
        }
    }
}
