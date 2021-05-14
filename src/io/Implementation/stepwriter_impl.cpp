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
McCAD::IO::STEPWriter::Impl::operator()(
        const McCAD::IO::STEPWriter::Impl::solidsMap& solidsMap){
    STEPControl_Writer writer;
    for (const auto& compound : solidsMap){
        for(const auto& solid : std::get<1>(compound)){
            writer.Transfer(solid, STEPControl_StepModelType::STEPControl_AsIs);
        }
        writer.Write(outputfileName.c_str());
    }
}

