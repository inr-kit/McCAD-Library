// McCAD
#include "inputdata_impl.hpp"
#include "tools_impl.hpp"
#include "decomposition_impl.hpp"

//McCAD::Decomposition::Decompose::Impl::Impl(){
//}

void McCAD::Decomposition::Decompose::Impl::getInputSolidsList()
{
  // Get the input solids list from the loaded STEP file.
  McCAD::General::InputData inputData;
  inputData.getInputSolidsList();
  // Retrieve the list from the InputData class.
  inputSolidsList = inputData.accessImpl()->inputSolidsList;
}

void McCAD::Decomposition::Decompose::Impl::splitInputSolids()
{  
  // Split the compound solids in the list.
  McCAD::Tools::Preprocessor preproc;
  splitInputSolidsList = preproc.accessImpl()->SplitCompSolids(inputSolidsList);
}

void McCAD::Decomposition::Decompose::Impl::decompose()
{
  // Loop ver the solids in the list and perform the decomposition
  int solidNumber = 0;
  for(int i = 1; i <= inputSolidsList->Length(); i++)
    {
      solidNumber++;
      std::cout <<"> Decomposing the "<< solidNumber <<" solid"<< std::endl;
      int level = 0;
      McCAD::Tools::Preprocessor preproc;
      TopoDS_Shape currentSolidShape = inputSolidsList->Value(i);
      /* Repair the geometry of solid */
      TopoDS_Shape newSolidShape = preproc.accessImpl()->removeSmallFaces(currentSolidShape); // Remove the small faces in solid
      TopoDS_Solid tempSolid = TopoDS::Solid(newSolidShape);
      TopoDS_Solid newSolid = preproc.accessImpl()->repairSolid(tmpSolid);      // Repair the solid
    }
}
