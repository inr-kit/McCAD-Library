//McCAD
#include "EdgesCombiner.hpp"
// OCC
#include <Standard.hxx>

void
McCAD::Decomposition::EdgesCombiner::operator()(
        std::vector<std::shared_ptr<McCAD::Geometry::Edge>>& edgesList){
    for (Standard_Integer i = 0; i < edgesList.size() - 1; ++i){
        for (Standard_Integer j = i+1; j < edgesList.size(); ++j){
            if (*(edgesList[i]) == *(edgesList[j])){
                edgesList.erase(edgesList.begin() + j);
                --j;
            }
        }
    }
}
