// McCAD
#include "stepreader_impl.hpp"
#include "ShapeView.hpp"
//OCC
#include <TDocStd_Document.hxx>
#include <TNaming_NamedShape.hxx>
#include <XCAFDoc_ShapeMapTool.hxx>
#include <TDataStd_Name.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <STEPControl_Writer.hxx>

McCAD::IO::STEPReader::Impl::Impl(const std::string& fileName)
    : fileName{fileName}, sequenceOfShape{new TopTools_HSequenceOfShape}{
    if(!std::filesystem::exists(fileName)){
        throw std::runtime_error("The specified input STEP file couldn't be found!."
                                 "\nHINT: check inputFileName on McCADInputConfig.txt");
    }
}

bool
McCAD::IO::STEPReader::Impl::iterateLabelChilds(const TDF_Label& aLabel,
                                                const TCollection_ExtendedString& aName){
    bool foundShapes = Standard_False;
    if(aLabel.HasChild()){
        int numChildren = aLabel.NbChildren();
        std::cout << "\nName: " << aName <<
                     "\nNumber of children: " << numChildren <<
                     "\nLabel: " << aLabel << std::endl;
        for(int aTag = 1; aTag <= numChildren; ++aTag){
            TDF_Label childLabel = aLabel.FindChild(aTag);
            if (childLabel.HasAttribute()){
                opencascade::handle<TDataStd_Name> name;
                if (childLabel.FindAttribute(TDataStd_Name::GetID(), name)
                        && childLabel.IsAttribute(TNaming_NamedShape::GetID())){
                    foundShapes = iterateLabelChilds(childLabel, name->Get());
                }
            }
        }
        if(!foundShapes && aLabel.IsAttribute(XCAFDoc_ShapeMapTool::GetID()))
            goto retrieve;
    } else {
        retrieve:
        opencascade::handle<TNaming_NamedShape> aShape;
        if (aLabel.FindAttribute(TNaming_NamedShape::GetID(), aShape)){
        std::cout << "\nName: " << aName <<
                     "\nLabel: " << aLabel  << std::endl;
        STEPControl_Writer writer;
        writer.Transfer(aShape->Get(), STEPControl_StepModelType::STEPControl_AsIs);
        Standard_Integer kk = 0;
        std::string filename = "test";
        std::string suffix = ".stp";
        while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
            ++kk;
        }
        filename += std::to_string(kk);
        filename += suffix;
        writer.Write(filename.c_str());
        }
        foundShapes = Standard_True;
    }
    return foundShapes;
}

void
McCAD::IO::STEPReader::Impl::getLabelInfo(const TDF_Label& aLabel){
    bool foundShapes = Standard_False;
    if(aLabel.HasChild()){
        int numChildren = aLabel.NbChildren();
        for(int aTag = 1; aTag <= numChildren; ++aTag){
            TDF_Label childLabel = aLabel.FindChild(aTag);
            if (childLabel.HasAttribute()){
                opencascade::handle<TDataStd_Name> name;
                if (childLabel.FindAttribute(TDataStd_Name::GetID(), name)
                        && name->Get() == "Shapes"){
                    // findattr XCAFDoc_ShapeTool
                    foundShapes = iterateLabelChilds(childLabel, name->Get());
                }
            }
        }
        if(!foundShapes)
            throw std::runtime_error("Error loading shapes from input file!");
    } else
        throw std::runtime_error("Error reading file, shapes could not be read!");
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
        TDF_Label label = document->Main();
        std::cout << "\nIs root: " << label.IsRoot() <<
                     "\nDepth: " << label.Depth() << std::endl;
        getLabelInfo(label);

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
