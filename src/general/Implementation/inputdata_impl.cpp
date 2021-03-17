// McCAD
#include "inputdata_impl.hpp"

McCAD::General::InputData::Impl::Impl()
  : inputSolidsList{std::make_shared<TopTools_HSequenceOfShape>()}{
}

McCAD::General::InputData::Impl::~Impl(){
}

void
McCAD::General::InputData::Impl::updateSize(){
    size = inputSolidsList->Length();
}
