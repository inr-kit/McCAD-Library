// C++
#include <filesystem>
#include <fstream>
#include <iostream>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <chrono>
#include <ctime>
// McCAD
#include "info.hpp"
#include "stepreader_impl.hpp"
#include "ShapeUtilities.hpp"
#include "StringUtilities.hpp"
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
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Builder.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_Tool.hxx>
#include <TopLoc_Datum3D.hxx>

McCAD::IO::STEPReader::Impl::Impl(const IO::InputConfig& inputConfig) :
    inputConfig{inputConfig},
    sequenceOfShape{std::make_shared<TopTools_HSequenceOfShape>()}{
    if(inputConfig.readConversion){
        fileName = inputConfig.conversionFileName;
    } else fileName = inputConfig.inputFileName;
    if(!std::filesystem::exists(fileName)){
        throw std::runtime_error("The specified input STEP file couldn't be found!"
                                 "\nHINT: check inputFileName on McCADInputConfig.i");
    }
}

McCAD::IO::STEPReader::Impl::~Impl(){}

void
McCAD::IO::STEPReader::Impl::readSTEP(){
    std::cout << " > Loading input solids." << std::endl;
    STEPCAFControl_Reader reader;
    STEPControl_Reader STEPReader = reader.Reader();
    // Create an application
    opencascade::handle<TDocStd_Document> document = new TDocStd_Document("txt");
    // Set reader parameters per user config.
    if(Interface_Static::RVal("read.precision.val") > inputConfig.precision){
        Interface_Static::SetIVal("read.precision.mode", 1);
        Interface_Static::SetRVal("read.precision.val", inputConfig.precision);
    }
    // Start reading contents of STEP file.
    auto readStatus = reader.ReadFile(fileName.c_str());
    if(readStatus == IFSelect_RetDone){
        reader.Transfer(document);
        TDF_Label label = document->Main();
        if(!getLabelInfo(label)){
            if(!basicReader(fileName)) goto failed;
        }
    } else {
        failed:
        Standard_Boolean failsOnly = Standard_False;
        STEPReader.PrintCheckLoad(failsOnly, IFSelect_ItemsByEntity);
        STEPReader.PrintCheckTransfer(failsOnly, IFSelect_ItemsByEntity);
        throw std::runtime_error("Error reading the input STEP file!");
    }
    // Write solid volumes to a text file.
    writeVolumes();
}

Standard_Boolean
McCAD::IO::STEPReader::Impl::getLabelInfo(const TDF_Label& rootLabel){
    Standard_Boolean foundShapes = Standard_False;
    if(rootLabel.HasChild()){
        for(TDF_ChildIterator it1 (rootLabel, Standard_False); it1.More(); it1.Next()) {
            TDF_Label childLabel = it1.Value();
            if (childLabel.HasAttribute()){
                if (childLabel.IsAttribute(XCAFDoc_ShapeTool::GetID()))
                    foundShapes = iterateLabelChilds(childLabel);
            }
        }
        if(!foundShapes || (sequenceOfShape->Length() != shapeNames.size())){
            return Standard_False;
        }
    } else return Standard_False;
    return foundShapes;
}

Standard_Boolean
McCAD::IO::STEPReader::Impl::iterateLabelChilds(const TDF_Label& aLabel){
    Standard_Boolean foundShapes = Standard_False;
    if(aLabel.HasChild()){
        for(TDF_ChildIterator it2 (aLabel, Standard_False); it2.More(); it2.Next()) {
            TDF_Label childLabel = it2.Value();
            if (childLabel.HasAttribute()){
                if (childLabel.IsAttribute(TNaming_NamedShape::GetID()))
                    foundShapes = iterateLabelChilds(childLabel);
            }
        }
        if(!foundShapes && aLabel.IsAttribute(XCAFDoc_ShapeMapTool::GetID())){
            goto retrieve;
        }
    } else {
        retrieve:
        opencascade::handle<TDataStd_Name> shapeName;
        opencascade::handle<TNaming_NamedShape> shape;
        if (aLabel.FindAttribute(TDataStd_Name::GetID(), shapeName) &&
                aLabel.FindAttribute(TNaming_NamedShape::GetID(), shape)){
            TCollection_AsciiString aLabelEntry;
            TDF_Tool::Entry(aLabel, aLabelEntry);
            sequenceOfShape->Append(shape->Get());
            shapeNames.push_back(shapeName->Get());
            shapesInfoMap.push_back(std::make_tuple(shape->Get(), shapeName->Get()));
            foundShapes = Standard_True;
            /*//
            TopLoc_Location labelLocation = XCAFDoc_ShapeTool::GetLocation(aLabel);
            std::cout << "shape vs label: " << shape->Get().Location().IsEqual(labelLocation) << std::endl;
            std::cout << "Trnasformation: " << "\n" <<
                         labelLocation.Transformation().Value(1,1) << " " <<
                         labelLocation.Transformation().Value(1,2) << " " <<
                         labelLocation.Transformation().Value(1,3) << " " <<
                         labelLocation.Transformation().Value(1,4) << " " << "\n" <<
                         labelLocation.Transformation().Value(2,1) << " " <<
                         labelLocation.Transformation().Value(2,2) << " " <<
                         labelLocation.Transformation().Value(2,3) << " " <<
                         labelLocation.Transformation().Value(2,4) << " " << "\n" <<
                         labelLocation.Transformation().Value(3,1) << " " <<
                         labelLocation.Transformation().Value(3,2) << " " <<
                         labelLocation.Transformation().Value(3,3) << " " <<
                         labelLocation.Transformation().Value(3,4) << std::endl;
            std::cout << "\nName: " << shapeName->Get() <<
                         "\nLabel: " << aLabel << std::endl;
            */
        }
    }
    return foundShapes;
}

Standard_Boolean
McCAD::IO::STEPReader::Impl::basicReader(const std::string& fileName){
    STEPControl_Reader STEPReader;
    // Set reader parameters per user config.
    if(Interface_Static::RVal("read.precision.val") > inputConfig.precision){
        Interface_Static::SetIVal("read.precision.mode", 1);
        Interface_Static::SetRVal("read.precision.val", inputConfig.precision);
    }
    auto readStatus = STEPReader.ReadFile(fileName.c_str());
    if(readStatus == IFSelect_RetDone){
        Standard_Integer numberOfRoots = STEPReader.NbRootsForTransfer();
        if (numberOfRoots != 0){
            for(Standard_Integer i = 1; i <= numberOfRoots; ++i){
                if(STEPReader.TransferRoot(i)){
                    TopoDS_Shape solidShape = STEPReader.Shape(i);
                    TopExp_Explorer explorer;
                    TopoDS_CompSolid compSolid;
                    TopoDS_Builder builder;
                    Standard_Integer counter{0};
                    for(explorer.Init(solidShape, TopAbs_SOLID); explorer.More(); explorer.Next()){
                        std::string tempName = "solid_" + std::to_string(++counter);
                        TCollection_ExtendedString shapeName{tempName.c_str()};
                        TopoDS_Solid tempSolid = TopoDS::Solid(explorer.Current());
                        builder.MakeCompSolid(compSolid);
                        builder.Add(compSolid, tempSolid);
                        shapesInfoMap.push_back(std::make_tuple(compSolid, shapeName));
                    }
                } else goto returnBack;
            }
            return Standard_True;
        } else goto returnBack;
    }
    returnBack:;
    return Standard_False;
}

/** ********************************************************************
* @brief   Writes volumes of shaped in the input STEP file to a text file
* @date    25/05/2022
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::IO::STEPReader::Impl::writeVolumes() {
    // Write solid volumes to a text file.
    std::string splitName = Tools::splitLine(fileName, '.')[0];
    std::string volumesFileName{ boost::str(boost::format("volumes_%s.i") % splitName) };
    std::ofstream volumeStream(volumesFileName.c_str());
    // Write volume file header
    auto timeStart{ std::chrono::system_clock::now() };
    std::time_t timeStart_t = std::chrono::system_clock::to_time_t(timeStart);
    volumeStream << boost::str(boost::format("McCAD v%s generated volumes file. ")
                               % McCAD::Info::McCADVersion) << std::ctime(&timeStart_t) <<
                    boost::str(boost::format("Column 1 is the volume [cubic %s] and column 2 is the shape name.")
                                             % inputConfig.units) << std::endl;
    std::string shapeData{};
    double shapeVolume;
    for (const auto& member : shapesInfoMap) {
        shapeVolume = Tools::calcVolume(std::get<0>(member));
        shapeData = boost::str(boost::format("%9.5E %s") % (shapeVolume * std::pow(1.0/inputConfig.conversionFactor, 3))
                                                         % std::get<1>(member));
        volumeStream << shapeData << std::endl;
    }
    volumeStream.close();
}
