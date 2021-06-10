// C++
#include <filesystem>
#include <fstream>
// McCAD
#include "stepreader_impl.hpp"
//OCC
#include <STEPCAFControl_Reader.hxx>
#include <STEPControl_Reader.hxx>
#include <Interface_Static.hxx>
#include <TDocStd_Document.hxx>
#include <TDataStd_Name.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ShapeMapTool.hxx>
#include <TDF_Attribute.hxx>
#include <TNaming_NamedShape.hxx>

McCAD::IO::STEPReader::Impl::Impl(const IO::InputConfig& inputConfig) :
    inputConfig{inputConfig},
    sequenceOfShape{std::make_shared<TopTools_HSequenceOfShape>()}{
    if(!std::filesystem::exists(inputConfig.inputFileName)){
        throw std::runtime_error("The specified input STEP file couldn't be found!"
                                 "\nHINT: check inputFileName on McCADInputConfig.txt");
    }
}

McCAD::IO::STEPReader::Impl::~Impl(){}

bool
McCAD::IO::STEPReader::Impl::iterateLabelChilds(const TDF_Label& aLabel,
                                                const TCollection_ExtendedString& aName){
    bool foundShapes = Standard_False;
    if(aLabel.HasChild()){
        int numChildren = aLabel.NbChildren();
        for(int aTag = 1; aTag <= numChildren; ++aTag){
            TDF_Label childLabel = aLabel.FindChild(aTag);
            if (childLabel.HasAttribute()){
                opencascade::handle<TDataStd_Name> dataName;
                if (childLabel.FindAttribute(TDataStd_Name::GetID(), dataName)
                         && childLabel.IsAttribute(TNaming_NamedShape::GetID())){
                    foundShapes = iterateLabelChilds(childLabel, dataName->Get());
                }
            }
        }
        if(!foundShapes && aLabel.IsAttribute(XCAFDoc_ShapeMapTool::GetID()))
            goto retrieve;
    } else {
        retrieve:
        /* // Debug
        std::cout << "\nName: " << aName <<
                     "\nparentName: " << parentName <<
                     "\nLabel: " << aLabel << std::endl;
        // Debug */
        opencascade::handle<TNaming_NamedShape> aShape;
        if (aLabel.FindAttribute(TNaming_NamedShape::GetID(), aShape)){
            sequenceOfShape->Append(aShape->Get());
            TCollection_ExtendedString shapeName{aName};
            shapeNames.push_back(shapeName);
            shapesInfoMap.push_back(std::make_tuple(aShape->Get(), shapeName));
            foundShapes = Standard_True;
        }
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
                opencascade::handle<TDataStd_Name> dataName;
                if (childLabel.FindAttribute(TDataStd_Name::GetID(), dataName)
                    && childLabel.IsAttribute(XCAFDoc_ShapeTool::GetID())){
                    foundShapes = iterateLabelChilds(childLabel, dataName->Get());
                }
            }
        }
        if(!foundShapes || (sequenceOfShape->Length() != shapeNames.size())){
            throw std::runtime_error("Error loading shapes from input file!");
        }
    } else
        throw std::runtime_error("Error loading shapes from input file!");
}

void
McCAD::IO::STEPReader::Impl::readSTEP(){
    std::cout << " > Populating the input solids list: " << std::endl;
    STEPCAFControl_Reader reader;
    STEPControl_Reader STEPReader = reader.Reader();
    opencascade::handle<TDocStd_Document> document = new TDocStd_Document("txt");
    // Set reader parameters per user config.
    if(Interface_Static::RVal("read.precision.val") > inputConfig.precision){
        Interface_Static::SetIVal("read.precision.mode", 1);
        Interface_Static::SetRVal("read.precision.val", inputConfig.precision);
    }
    auto readStatus = reader.ReadFile(inputConfig.inputFileName.c_str());
    if(readStatus == IFSelect_RetDone){
        reader.Transfer(document);
        TDF_Label label = document->Main();
        getLabelInfo(label);
    } else {
        Standard_Boolean failsOnly = Standard_False;
        STEPReader.PrintCheckLoad(failsOnly, IFSelect_ItemsByEntity);
        STEPReader.PrintCheckTransfer(failsOnly, IFSelect_ItemsByEntity);
        throw std::runtime_error("Error reading input STEP file!");
    }
}
