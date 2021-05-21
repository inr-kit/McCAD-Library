// C++
#include <filesystem>
// McCAD
#include "stepwriter_impl.hpp"
// OCC
#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <STEPControl_Writer.hxx>
#include <NCollection_Vector.hxx>
#include <STEPCAFControl_Writer.hxx>
#include <TDocStd_Document.hxx>
#include <TDataStd_Name.hxx>
#include <TDF_Label.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TNaming_NamedShape.hxx>
#include <AppStd_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <TopoDS.hxx>

McCAD::IO::STEPWriter::Impl::Impl(const std::string& fileName) :
    outputfileName{fileName}{
    if(std::filesystem::exists(fileName)){
        std::string oldFileName{"old_" + fileName};
        std::rename(fileName.c_str(), oldFileName.c_str());
    }
}

/*
void
McCAD::IO::STEPWriter::Impl::operator()(
        const McCAD::IO::STEPWriter::Impl::solidsMap& solidsMap){
    STEPControl_Writer STEPWriter;
    TopoDS_Builder builder;
    for (const auto& compound : solidsMap){
        TopoDS_Compound compoundToWrite;
        builder.MakeCompound(compoundToWrite);
        for(const auto& solid : std::get<1>(compound)){
            builder.Add(compoundToWrite, solid);
        }
        STEPWriter.Transfer(compoundToWrite, STEPControl_StepModelType::STEPControl_AsIs);
    }
    STEPWriter.Write(outputfileName.c_str());
}
*/
void
McCAD::IO::STEPWriter::Impl::operator()(
        const McCAD::IO::STEPWriter::Impl::solidsMap& solidsMap){
    STEPCAFControl_Writer writer;
    STEPControl_Writer STEPWriter = writer.Writer();
    opencascade::handle<TDocStd_Document> document = new TDocStd_Document("txt");
    opencascade::handle<XCAFDoc_ShapeTool> shapeTool = XCAFDoc_DocumentTool::ShapeTool(document->Main());
    opencascade::handle<TDataStd_Name> compoundName = new TDataStd_Name();
    TopoDS_Builder builder;
    TopoDS_Compound compoundToWrite;
    for (const auto& compound : solidsMap){
        builder.MakeCompound(compoundToWrite);
        for(const auto& solid : std::get<1>(compound)){
            builder.Add(compoundToWrite, solid);
        }
        TDF_Label compoundLabel = shapeTool->AddShape(compoundToWrite, Standard_True);
        compoundName->Set(compoundLabel, std::get<0>(compound));
    }
    writer.Transfer(document, STEPControl_StepModelType::STEPControl_AsIs);
    writer.Write(outputfileName.c_str());
}
