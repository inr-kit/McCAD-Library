// McCAD
#include "stepwriter_impl.hpp"

McCAD::IO::STEPWriter::Impl::Impl(const std::string& fileName){
  if(std::filesystem::exists(fileName))
    {
      outputfileName = "./test_output.step";
    }
}

void
McCAD::IO::STEPWriter::Impl::operator()(const Handle_TopTools_HSequenceOfShape& shapes){
  STEPControl_Writer writer;
  for (Standard_Integer i = 1; i <= shapes->Length(); ++i)
    {
      writer.Transfer(shapes->Value(i), STEPControl_StepModelType::STEPControl_AsIs);
    }
  writer.Write(outputfileName.c_str());
}
