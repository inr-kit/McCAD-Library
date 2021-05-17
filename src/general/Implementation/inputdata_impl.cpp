// McCAD
#include "inputdata_impl.hpp"

McCAD::General::InputData::Impl::Impl(){
}

McCAD::General::InputData::Impl::~Impl(){
}

void
McCAD::General::InputData::Impl::updateSize(){
    size = outputShapesMap.size();
}
