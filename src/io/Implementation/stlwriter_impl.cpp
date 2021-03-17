// McCAD
#include "stlwriter_impl.hpp"
// OCC
#include <StlAPI_Writer.hxx>

McCAD::IO::STLWriter::Impl::Impl(const std::string& fileName) :
    conversionfileName{fileName}{
    if(std::filesystem::exists(fileName.c_str())){
        std::string oldFileName{"old_" + fileName};
        std::rename(fileName.c_str(), oldFileName.c_str());
    }
}

McCAD::IO::STLWriter::Impl::~Impl(){
}

void
McCAD::IO::STLWriter::Impl::operator()(
        const std::shared_ptr<TopTools_HSequenceOfShape>& shapes){
    // Create an empty file to write to
    std::ofstream outputFile(conversionfileName.c_str(), std::ios_base::app); // append instead of overwrite
    StlAPI_Writer writer;
    std::string tmpFile = "tmp.stl";
    for (Standard_Integer i = 1; i <= shapes->Length(); ++i){
        writer.Write(shapes->Value(i), tmpFile.c_str());
        std::ifstream tmpOutputFile(tmpFile.c_str());
        outputFile.seekp(0, std::ios_base::end);
        outputFile << tmpOutputFile.rdbuf();
    }
    std::remove(tmpFile.c_str());
}

