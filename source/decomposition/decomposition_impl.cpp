// McCAD
#include "decomposition_impl.hpp"
#include "inputdata_impl.hpp"
#include "tools_impl.hpp"

McCAD::Decomposition::Decompose::Impl::Impl(const McCAD::General::InputData& inputData) : splitInputSolidsList{new TopTools_HSequenceOfShape}{
  if (inputData.accessImpl()->inputSolidsList->Length() >= 1)
    {
      std::cout << "  - Length of input solids list: " << inputData.accessImpl()->inputSolidsList->Length() << std::endl;
      std::cout << " > Spliting input solids" << std::endl;
      // Split input compound input solids.
      splitInputSolids(inputData.accessImpl()->inputSolidsList);
      std::cout << "  - Length of split input solids list: " << splitInputSolidsList->Length() << std::endl;
      std::cout << " > Decomposing solids" << std::endl;
      // perform the decomposition.
      perform();
    }
  else
    {
      throw std::runtime_error("Input solids list is empty!.");
    }
}

void
McCAD::Decomposition::Decompose::Impl::splitInputSolids(const Handle_TopTools_HSequenceOfShape& inputSolidsList){
  // Split the compound solids in the list.
  for (Standard_Integer i=1; i <= inputSolidsList->Length(); i++)
    {
      if ((inputSolidsList->Value(i)).ShapeType() == TopAbs_COMPSOLID ||
	  (inputSolidsList->Value(i)).ShapeType() == TopAbs_COMPOUND)
	{
	  TopExp_Explorer ex;
	  for (ex.Init(inputSolidsList->Value(i), TopAbs_SOLID); ex.More(); ex.Next())
	    {
	      TopoDS_Solid tmpSol = TopoDS::Solid(ex.Current());
	      splitInputSolidsList->Append(tmpSol);
	    }
	}
      else if ((inputSolidsList->Value(i)).ShapeType() == TopAbs_SOLID)
	{
	  splitInputSolidsList->Append(inputSolidsList->Value(i));
	}
    }
}

void
McCAD::Decomposition::Decompose::Impl::perform(){
  // Loop over the solids in the list and perform the decomposition
  McCAD::Tools::Preprocessor preproc;
  Standard_Integer solidNumber = 0;
  for(Standard_Integer i = 1; i <= splitInputSolidsList->Length(); i++)
    {
      solidNumber++;
      std::cout << "  - Decomposing solid # "<< solidNumber << std::endl;
      Standard_Integer level = 0;
      TopoDS_Shape currentSolidShape = splitInputSolidsList->Value(i);
      
      // Repair the geometry of solid /
      TopoDS_Shape newSolidShape = preproc.accessImpl()->removeSmallFaces(currentSolidShape);
      TopoDS_Solid tempSolid = TopoDS::Solid(newSolidShape);
      TopoDS_Solid newSolid = preproc.accessImpl()->repairSolid(tempSolid);
      TopoDS_Solid repairedSolid = preproc.accessImpl()->genericFix(newSolid);
      // Perform decomposition on the repaired solid.
    }
}
