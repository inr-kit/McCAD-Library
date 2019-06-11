// McCAD
#include "inputdata_impl.hpp"
#include "decomposition_impl.hpp"

//McCAD::Decomposition::Decompose::Impl::Impl(){
//}

void McCAD::Decomposition::Decompose::Impl::getInputSolidsList(){
  McCAD::General::InputData getInputSolidsList;
  getInputSolidsList.getInputSolidsList();
  //McCAD::General::InputData::Impl pinputSolidsList;
  //theinputSolidsList = pinputSolidsList->inputSolidsList;
  McCAD::General::InputData inputData;
  inputSolidsList = inputData.accessImpl()->inputSolidsList;
}


