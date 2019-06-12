// McCAD
#include "inputdata_impl.hpp"
#include "decomposition_impl.hpp"
#include "tools_impl.hpp"

//McCAD::Decomposition::Decompose::Impl::Impl(){
//}

void McCAD::Decomposition::Decompose::Impl::getInputSolidsList(){
  // Get the inpul solids list from the loaded STEP file.
  McCAD::General::InputData getInputSolidsList;
  getInputSolidsList.getInputSolidsList();
  // Retrieve the list from the InputData class.
  McCAD::General::InputData inputData;
  inputSolidsList = inputData.accessImpl()->inputSolidsList;
}

void McCAD::Decomposition::Decompose::Impl::splitInputSolids(){  
  // Split the compound solids in the list.
  McCAD::Tools::Preprocessor preproc;
  splitInputSolidsList = preproc.accessImpl()->SplitCompSolids(inputSolidsList)
}


