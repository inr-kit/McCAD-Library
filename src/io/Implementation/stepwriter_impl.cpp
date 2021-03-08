// McCAD
#include "stepwriter_impl.hpp"
// OCC
#include <STEPControl_Writer.hxx>

McCAD::IO::STEPWriter::Impl::Impl(const std::string& fileName) :
    outputfileName{fileName}{
    if(std::filesystem::exists(fileName)){
        std::string oldFileName{"old_" + fileName};
        std::rename(fileName.c_str(), oldFileName.c_str());
    }
}

void
McCAD::IO::STEPWriter::Impl::operator()(const Handle_TopTools_HSequenceOfShape& shapes){
    STEPControl_Writer writer;
    for (Standard_Integer i = 1; i <= shapes->Length(); ++i){
        writer.Transfer(shapes->Value(i),STEPControl_StepModelType::STEPControl_AsIs);
    }
    writer.Write(outputfileName.c_str());
}

