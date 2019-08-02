// McCAD
#include "decomposition_impl.hpp"

McCAD::Decomposition::Decompose::Impl::Impl(const McCAD::General::InputData& inputData) : splitInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}, rejectedInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}{
  auto inputSolidsList = inputData.accessImpl()->inputSolidsList;
  if (inputSolidsList->Length() >= 1)
    {
      std::cout << "   - Found " << inputSolidsList->Length() << " solid(s) in the input file." << std::endl;
      std::cout << " > Spliting compound input solids" << std::endl;
      // Split compound input solids.
      splitInputSolids(inputSolidsList);
      std::cout << "   - There are " << splitInputSolidsList->Length() << " solid(s) in the split solids list." <<std::endl;
      std::cout << " > Decomposing solid(s)" << std::endl;
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
  // Split compound solids in the input solids list.
  for (Standard_Integer i = 1; i <= inputSolidsList->Length(); i++)
    {
      if ((inputSolidsList->Value(i)).ShapeType() == TopAbs_COMPSOLID ||
	  (inputSolidsList->Value(i)).ShapeType() == TopAbs_COMPOUND)
	{
	  std::cout << "   - Solid # " << i << " is a compound solid" << std::endl; 
	  TopExp_Explorer explorer(inputSolidsList->Value(i), TopAbs_SOLID);
	  for (; explorer.More(); explorer.Next())
	    {
	      TopoDS_Solid tempSolid = TopoDS::Solid(explorer.Current());
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
  for(Standard_Integer solidNumber = 1; solidNumber <= splitInputSolidsList->Length(); solidNumber++)
    {
      solidShape = splitInputSolidsList->Value(solidNumber);
      solid = TopoDS::Solid(solidShape);
      // Check the boudary surfaces of the solid.
      // If the solid has spline surfaces, torus it cannot be processed by the current version.
      Standard_Boolean rejectCondition = preproc.accessImpl()->checkBndSurfaces(solid);
      if (rejectCondition)
	{
	  rejectedInputSolidsList->Append(solidShape);
	  continue;
	}
      else
	{
	  // Repair the geometry of solid
	  preproc.accessImpl()->removeSmallFaces(solidShape);
	  solid = TopoDS::Solid(solidShape);
	  preproc.accessImpl()->repairSolid(solid);
	  preproc.accessImpl()->genericFix(solid);
	  
	  // Perform decomposition on the repaired solid.
	  std::cout << "   - Decomposing solid # " << solidNumber << std::endl;
	  std::unique_ptr<McCAD::Decomposition::DecomposeSolid> decomposedSolid = std::make_unique<McCAD::Decomposition::DecomposeSolid>();
	  if (decomposedSolid->accessDSImpl()->initiate(solid))
	    {
	      std::cout << "length: " << decomposedSolid->accessDSImpl()->splitSolidList->Length() << std::endl;
	      resultSolidsList = std::move(decomposedSolid->accessDSImpl()->splitSolidList);
	      /*
	      for (Standard_Integer i = 1; i <= decomposedSolid->accessDSImpl()->splitSolidList->Length(); ++i)
		{
		  std::cout << i << std::endl;
		  resultSolidsList->Append(decomposedSolid->accessDSImpl()->splitSolidList->Value(i));
		}
	      */
	    }
	  else
	    {
	      rejectedInputSolidsList->Append(solidShape);
	    }
	}
    }
  std::cout << "   - There are " << rejectedInputSolidsList->Length() << " rejected solid(s)."<< std::endl;
  std::cout << "length of split solids list: " << resultSolidsList->Length() << std::endl;
}
