// McCAD
#include "stepreader_impl.hpp"
#include "ShapeView.hpp"
//OCC
#include <TDocStd_Document.hxx>
#include <TDF_Label.hxx>
#include <TDataStd_Name.hxx>
#include <STEPCAFControl_Reader.hxx>

McCAD::IO::STEPReader::Impl::Impl(const std::string& fileName)
    : fileName{fileName}, sequenceOfShape{new TopTools_HSequenceOfShape}{
    if(!std::filesystem::exists(fileName)){
        throw std::runtime_error("The specified input STEP file couldn't be found!."
                                 "\nHINT: check inputFileName on McCADInputConfig.txt");
    }
}

void
McCAD::IO::STEPReader::Impl::readSTEP(){
    std::cout << " > Populating the input solids list: " << std::endl;
    STEPCAFControl_Reader reader;
    STEPControl_Reader STEPReader = reader.Reader();
    opencascade::handle<TDocStd_Document> document = new TDocStd_Document("txt");
    auto readStatus = reader.ReadFile(fileName.c_str());
    if(readStatus == IFSelect_RetDone){
        reader.Transfer(document);
        auto TDFLabel = document->Main();
        std::cout << "num children: " << TDFLabel.NbChildren() <<
                     "\nnum attr: " << TDFLabel.NbAttributes() <<
                     "\nDepth: " << TDFLabel.Depth() <<
                     "\nRoot: " << TDFLabel.IsRoot() << std::endl;
        opencascade::handle<TDataStd_Name> name;
        if(TDFLabel.FindAttribute(TDataStd_Name::GetID(), name))
            std::cout << ">>Name: " << name->Get() << "\n";
        else std::cout << ">>Name: ---\n";
        Standard_Integer numberOfRoots = STEPReader.NbRootsForTransfer();
        if (numberOfRoots != 0){
            for(Standard_Integer i = 1; i <= numberOfRoots; ++i){
                if(STEPReader.TransferRoot(i)){
                    TopoDS_Shape shape = STEPReader.Shape(i);
                    TopoDS_CompSolid compSolid;
                    TopoDS_Builder builder;
                    for(const auto& solid : detail::ShapeView<TopAbs_SOLID>{shape}){
                        TopoDS_Solid tempSolid = TopoDS::Solid(solid);
                        builder.MakeCompSolid(compSolid);
                        builder.Add(compSolid, tempSolid);
                        sequenceOfShape->Append(compSolid);
                    }
                }
                else{
                    throw std::runtime_error("Error reading file, a shape could not be read!");
                }
            }
        }
    } else {
        Standard_Boolean failsOnly = Standard_False;
        STEPReader.PrintCheckLoad(failsOnly, IFSelect_ItemsByEntity);
        STEPReader.PrintCheckTransfer(failsOnly, IFSelect_ItemsByEntity);
        throw std::runtime_error("Error reading file!");
    }
}
