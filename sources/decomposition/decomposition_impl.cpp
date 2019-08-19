// McCAD
#include "decomposition_impl.hpp"

McCAD::Decomposition::Decompose::Impl::Impl(const McCAD::General::InputData& inputData) : splitInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}, rejectedInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}, resultSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}, rejectedsubSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}{
  // Get input solids list from the Input Data object.
  auto inputSolidsList = inputData.accessImpl()->inputSolidsList;
  if (inputSolidsList->Length() > 0)
    {
      std::cout << "   - Found " << inputSolidsList->Length() << " solid(s) in the input file." << std::endl;
      std::cout << " > Spliting compound input solids" << std::endl;
      // Split compound input solids.
      splitInputSolids(inputSolidsList);
      std::cout << "   - There are " << splitInputSolidsList->Length() << " solid(s) in the split solids list." <<std::endl;
      std::cout << " > Decomposing solid(s)" << std::endl;
      // Perform the decomposition.
      perform();
    }
  else
    {
      // Throw an exception.
      throw std::runtime_error("Input solids list is empty!.");
    }
}

McCAD::Decomposition::Decompose::Impl::~Impl(){
}

void
McCAD::Decomposition::Decompose::Impl::splitInputSolids(const Handle_TopTools_HSequenceOfShape& inputSolidsList){
  // Split compound solids in the input solids list.
  for (Standard_Integer i = 1; i <= inputSolidsList->Length(); ++i)
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
  // Loop over the solids in the split solids list and perform the decomposition
  for(Standard_Integer solidNumber = 1; solidNumber <= splitInputSolidsList->Length(); ++solidNumber)
    {
      std::unique_ptr<Solid> solid = std::make_unique<Solid>();
      solid->accessSImpl()->initiate(splitInputSolidsList->Value(solidNumber));
      // If the solid has spline or tori surfaces it cannot be processed by the current version of the code.
      if (solid->accessSImpl()->isTorus || solid->accessSImpl()->isSpline)
	{
	  rejectedInputSolidsList->Append(solid->accessSImpl()->solidShape);
	}
      else
	{
	  // Repair the geometry of solid
	  solid->accessSImpl()->repairSolid;
	  // Perform decomposition on the repaired solid.
	  std::cout << "   - Decomposing solid # " << solidNumber << std::endl;
	  std::unique_ptr<DecomposeSolid> decomposeSolid = std::make_unique<DecomposeSolid>();
	  if (decomposeSolid->accessDSImpl()->perform(solid))
	    {
	      Standard_Integer lengthList = solid->accessSImpl()->splitSolidList->Length();
	      for (Standard_Integer i = 1; i <= lengthList; ++i)
		{
		  resultSolidsList->Append(solid->accessSImpl()->splitSolidList->Value(i));
		}
	      if (solid->accessSImpl()->rejectedsubSolidsList->Length() >= 1)
                {
                  for (Standard_Integer j = 1; j <= solid->SImpl()->rejectedsubSolidsList->Length(); ++j)
                    {
                      rejectedsubSolidsList->Append(solid->accessSImpl()->rejectedsubSolidsList->Value(j));
                    }
		}
	    }
	  else
	    {
	      rejectedInputSolidsList->Append(solid->accessSImpl()->solidShape);
	    }
	}
    }
  std::cout << "   - There are " << rejectedInputSolidsList->Length() << " rejected solid(s)."<< std::endl;
  std::cout << "   - There are " << resultSolidsList->Length() << " result solid(s)." << std::endl;
  std::cout << "   - There are " << rejectedsubSolidsList->Length() << " rejected subsolid(s)." << std::endl;
}
