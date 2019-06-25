// McCAD
#include "stepreader_impl.hpp"

McCAD::IO::STEPReader::Impl::Impl(const std::string& fileName)
    : fileName{fileName}
    , sequenceOfShape{new TopTools_HSequenceOfShape}{
      
    if(!std::filesystem::exists(fileName)){
      throw std::runtime_error("The specified STEP file couldn't be found!");
    }
}

void
McCAD::IO::STEPReader::Impl::operator()(){
    std::cout << " > Populating the input solids list: " << std::endl;
    ofstream runLog;
    runLog.open("runLog.txt");
    STEPControl_Reader reader;

    if(reader.ReadFile(fileName.c_str()) == IFSelect_RetDone)
      {
	Standard_Boolean failsonly = Standard_False;
	reader.PrintCheckLoad(failsonly, IFSelect_ItemsByEntity);
	reader.PrintCheckTransfer(failsonly, IFSelect_ItemsByEntity);
	
	Standard_Integer numberOfRoots = reader.NbRootsForTransfer();
	if (numberOfRoots != 0)
	  {
	    for(Standard_Integer i = 1;
		i <= numberOfRoots; ++i)
	      {
		if(reader.TransferRoot(i))
		  {
		    TopoDS_Shape  solidShape = reader.Shape(i);
		    TopExp_Explorer explorer;
		    TopoDS_CompSolid compSolid;
                    TopoDS_Builder builder;
		    for(explorer.Init(solidShape, TopAbs_SOLID);
			explorer.More(); explorer.Next())
		      {
			TopoDS_Solid tempSolid = TopoDS::Solid(explorer.Current());
			builder.MakeCompSolid(compSolid);
                        builder.Add(compSolid,tempSolid);
                        sequenceOfShape->Append(compSolid);
		      }
		  }
		else
		  {
		    throw std::runtime_error("Error reading file, a shape could not be read!");
		  }
	      }
	  }
      }
    runLog.close();
}
