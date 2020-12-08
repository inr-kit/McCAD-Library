// McCAD
#include "inputdata_impl.hpp"

McCAD::General::InputData::Impl::Impl()
  : inputSolidsList{new TopTools_HSequenceOfShape}{
}

McCAD::General::InputData::Impl::~Impl(){
}

void
McCAD::General::InputData::Impl::updateSize(){
    size = inputSolidsList->Length();
}
