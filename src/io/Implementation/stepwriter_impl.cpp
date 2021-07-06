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
#include <Interface_Static.hxx>
#include <TopoDS.hxx>

McCAD::IO::STEPWriter::Impl::Impl(const IO::InputConfig& inputConfig) :
    outputFileName{inputConfig.outputFileName}, inputConfig{inputConfig}{
    if(std::filesystem::exists(outputFileName)){
        std::string oldFileName{"old_" + outputFileName};
        std::rename(outputFileName.c_str(), oldFileName.c_str());
    }
}

McCAD::IO::STEPWriter::Impl::~Impl(){}

/*
void
McCAD::IO::STEPWriter::Impl::operator()(
        const McCAD::IO::STEPWriter::Impl::solidsMap& solidsMap){
    STEPControl_Writer STEPWriter;
    // Define writer parameters.
    if(inputConfig.units != "mm"){
        Interface_Static::SetCVal("write.step.unit", inputConfig.units.c_str());
    }
    if(Interface_Static::RVal("write.precision.val") > inputConfig.precision){
        Interface_Static::SetIVal("write.precision.mode", 1);
        Interface_Static::SetRVal("write.precision.val", inputConfig.precision);
    }
    Interface_Static::SetIVal("write.step.assembly", 2); // Auto
    Interface_Static::SetIVal("write.step.schema", 4); // AP2014 IS (2002)
    TopoDS_Builder builder;
    for (const auto& compound : solidsMap){
        //Interface_Static::SetCVal("write.step.product.name", std::string(std::get<0>(compound)));
        TopoDS_Compound compoundToWrite;
        builder.MakeCompound(compoundToWrite);
        for(const auto& solid : std::get<1>(compound)){
            builder.Add(compoundToWrite, solid);
        }
        STEPWriter.Transfer(compoundToWrite, STEPControl_StepModelType::STEPControl_ManifoldSolidBrep);
    }
    STEPWriter.Write(outputFileName.c_str());
}
*/

void
McCAD::IO::STEPWriter::Impl::operator()(
        const McCAD::IO::STEPWriter::Impl::solidsMap& solidsMap){
    STEPCAFControl_Writer writer;
    STEPControl_Writer STEPWriter = writer.Writer();
    if(inputConfig.units != "mm"){
        Interface_Static::SetCVal("write.step.unit", inputConfig.units.c_str());
    }
    if(Interface_Static::RVal("write.precision.val") > inputConfig.precision){
        Interface_Static::SetIVal("write.precision.mode", 1);
        Interface_Static::SetRVal("write.precision.val", inputConfig.precision);
    }
    Interface_Static::SetIVal("write.step.assembly", 2); // Auto
    Interface_Static::SetIVal("write.step.schema", 4); // AP2014 IS (2002)
    opencascade::handle<TDocStd_Document> document = new TDocStd_Document("txt");
    opencascade::handle<XCAFDoc_ShapeTool> shapeTool = XCAFDoc_DocumentTool::ShapeTool(document->Main());
    opencascade::handle<TDataStd_Name> compoundName = new TDataStd_Name();
    TopoDS_Builder builder;
    TopoDS_Compound compoundToWrite;
    for (const auto& compound : solidsMap){
        TDF_Label compoundLabel = shapeTool->NewShape();
        compoundName->Set(compoundLabel, std::get<0>(compound));
        builder.MakeCompound(compoundToWrite);
        for(const auto& solid : std::get<1>(compound)){
            builder.Add(compoundToWrite, solid);
        }
        compoundLabel = shapeTool->AddComponent(compoundLabel, compoundToWrite, Standard_False);
    }
    shapeTool->UpdateAssemblies();
    writer.Transfer(document, STEPControl_StepModelType::STEPControl_AsIs);
    writer.Write(outputFileName.c_str());
}
