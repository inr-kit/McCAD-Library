// McCAD
#include "decomposition_impl.hpp"
//#include "decomposeSolid_impl.hpp"
#include "inputdata_impl.hpp"
#include "tools_impl.hpp"

McCAD::Decomposition::Decompose::Impl::Impl(const McCAD::General::InputData& inputData) : splitInputSolidsList{new TopTools_HSequenceOfShape}, rejectedInputSolidsList{new TopTools_HSequenceOfShape}{
  auto inputSolidsList = inputData.accessImpl()->inputSolidsList;
  if (inputSolidsList->Length() >= 1)
    {
      std::cout << "   - Length of input solids list: " << inputSolidsList->Length() << std::endl;
      std::cout << " > Spliting compound input solids" << std::endl;
      // Split input compound input solids.
      splitInputSolids(inputSolidsList);
      std::cout << "   - Length of split input solids list: " << splitInputSolidsList->Length() << std::endl;
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
  for (Standard_Integer i = 1; i <= inputSolidsList->Length(); i++)
    {
      if ((inputSolidsList->Value(i)).ShapeType() == TopAbs_COMPSOLID ||
	  (inputSolidsList->Value(i)).ShapeType() == TopAbs_COMPOUND)
	{
	  std::cout << "   - Solid # " << i << " is a compound solid" << std::endl; 
	  TopExp_Explorer explorer(inputSolidsList->Value(i), TopAbs_SOLID);
	  for (; explorer.More(); explorer.Next())
	    {
	      const TopoDS_Solid& tempSolid = TopoDS::Solid(explorer.Current());
	      splitInputSolidsList->Append(tempSolid);
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
  //McCAD::Decomposition::DecomposeSolid decomposedSolid;
  for(Standard_Integer solidNumber = 1; solidNumber <= splitInputSolidsList->Length(); solidNumber++)
    {
      Standard_Integer level = 0;
      TopoDS_Shape currentSolidShape = splitInputSolidsList->Value(solidNumber);
      TopoDS_Solid tempSolid = TopoDS::Solid(currentSolidShape);
      // Check the boudary surfaces of the solid.
      // If the solid has spline surfaces, torus it cannot be processed by the current version.
      Standard_Boolean rejectCondition = preproc.accessImpl()->checkBndSurfaces(tempSolid);
      if (rejectCondition)
	{
	  rejectedInputSolidsList->Append(splitInputSolidsList->Value(solidNumber));
	  continue;
	}
      else
	{
	  // Repair the geometry of solid
	  TopoDS_Shape newSolidShape = preproc.accessImpl()->removeSmallFaces(currentSolidShape);
	  TopoDS_Solid tempSolid = TopoDS::Solid(newSolidShape);
	  TopoDS_Solid newSolid = preproc.accessImpl()->repairSolid(tempSolid);
	  TopoDS_Solid repairedSolid = preproc.accessImpl()->genericFix(newSolid);
	  
	  auto deflection = preproc.accessImpl()->calcMeshDeflection(repairedSolid);
	  // Perform decomposition on the repaired solid.
	  std::cout << "   - Decomposing solid # "<< solidNumber << std::endl;
	  //decomposeSolid.accessImpl()->;
	}
    }
  std::cout << "   - Rejected: " << rejectedInputSolidsList->Length() << " solids."<< std::endl;
}
