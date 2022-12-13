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
#include "ShapeView.hpp"
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
#include <GeomAdaptor_Surface.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>

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
        bool failsOnly = false;
        STEPReader.PrintCheckLoad(failsOnly, IFSelect_ItemsByEntity);
        STEPReader.PrintCheckTransfer(failsOnly, IFSelect_ItemsByEntity);
        throw std::runtime_error("Error reading the input STEP file!");
    }
    // Write solid volumes to a text file.
    writeVolumes();
    // Output surfaces tally to screen.
    writeSurfacesTally();
}

bool
McCAD::IO::STEPReader::Impl::getLabelInfo(const TDF_Label& rootLabel){
    bool foundShapes = false;
    if(rootLabel.HasChild()){
        for(TDF_ChildIterator it1 (rootLabel, false); it1.More(); it1.Next()) {
            TDF_Label childLabel = it1.Value();
            if (childLabel.HasAttribute()){
                if (childLabel.IsAttribute(XCAFDoc_ShapeTool::GetID()))
                    foundShapes = iterateLabelChilds(childLabel);
            }
        }
        if(!foundShapes || (sequenceOfShape->Length() != shapeNames.size())){
            return false;
        }
    } else return false;
    return foundShapes;
}

bool
McCAD::IO::STEPReader::Impl::iterateLabelChilds(const TDF_Label& aLabel){
    bool foundShapes = false;
    if(aLabel.HasChild()){
        for(TDF_ChildIterator it2 (aLabel, false); it2.More(); it2.Next()) {
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
            foundShapes = true;
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

bool
McCAD::IO::STEPReader::Impl::basicReader(const std::string& fileName){
    STEPControl_Reader STEPReader;
    // Set reader parameters per user config.
    if(Interface_Static::RVal("read.precision.val") > inputConfig.precision){
        Interface_Static::SetIVal("read.precision.mode", 1);
        Interface_Static::SetRVal("read.precision.val", inputConfig.precision);
    }
    auto readStatus = STEPReader.ReadFile(fileName.c_str());
    if(readStatus == IFSelect_RetDone){
        int numberOfRoots = STEPReader.NbRootsForTransfer();
        if (numberOfRoots != 0){
            for(int i = 1; i <= numberOfRoots; ++i){
                if(STEPReader.TransferRoot(i)){
                    TopoDS_Shape solidShape = STEPReader.Shape(i);
                    TopExp_Explorer explorer;
                    TopoDS_CompSolid compSolid;
                    TopoDS_Builder builder;
                    int counter{0};
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
            return true;
        } else goto returnBack;
    }
    returnBack:;
    return false;
}

/** ********************************************************************
* @brief    Writes volumes of shapes in the input STEP file to a text file
* @date     25/05/2022
* @modified 09/06/2022
* @author   Moataz Harb
* **********************************************************************/
void
McCAD::IO::STEPReader::Impl::writeVolumes() {
    // Write solid volumes to a text file.
    std::string splitName = Tools::splitLine(fileName, '.')[0];
    std::string volumesFileName{ boost::str(boost::format("%sVolumes.i") % splitName) };
    std::ofstream volumeStream(volumesFileName.c_str());
    // Write volumes file header
    auto timeStart{ std::chrono::system_clock::now() };
    std::time_t timeStart_t = std::chrono::system_clock::to_time_t(timeStart);
    volumeStream << boost::str(boost::format("McCAD v%s generated volumes file / ") % McCAD::Info::McCADVersion) << 
                    std::ctime(&timeStart_t) <<
                    boost::str(boost::format("File contents : a list of CAD solid volumes from the input STEP file as loaded by OCCT.")) <<
                    boost::str(boost::format("\nColumn 1 is the volume [cubic %s] and column 2 is the shape name.")
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

/** ********************************************************************
* @brief    Writes tally of surfaces count & type to screen.
* @date     12/12/2022
* @modified 12/12/2022
* @author   Moataz Harb
* **********************************************************************/
void
McCAD::IO::STEPReader::Impl::writeSurfacesTally() {
    int planes{0}, cylinders{0}, tori{0}, cones{0}, spheres{0}, BezierSurface{0}, BSpline{0}, 
        SurfaceOfRevolution{0}, SurfaceOfExtrusion{0}, OffsetSurface{0}, unknown{0};
    for (const auto& member : shapesInfoMap) {
        for (const auto& solid : detail::ShapeView<TopAbs_SOLID>{ std::get<0>(member) }) {
            for (const auto& face : detail::ShapeView<TopAbs_FACE>{ solid }) {
                GeomAdaptor_Surface surfAdaptor(BRep_Tool::Surface(face));
                switch (surfAdaptor.GetType()) {
                case GeomAbs_Plane:
                    planes += 1;
                    break;
                case GeomAbs_Cylinder:
                    cylinders += 1;
                    break;
                case GeomAbs_Torus:
                    tori += 1;
                    break;
                case GeomAbs_Cone:
                    cones += 1;
                    break;
                case GeomAbs_Sphere:
                    spheres += 1;
                    break;
                case GeomAbs_BezierSurface:
                    BezierSurface += 1;
                    break;
                case GeomAbs_BSplineSurface:
                    BSpline += 1;
                    break;
                case GeomAbs_SurfaceOfRevolution:
                    SurfaceOfRevolution += 1;
                    break;
                case GeomAbs_SurfaceOfExtrusion:
                    SurfaceOfExtrusion += 1;
                    break;
                case GeomAbs_OffsetSurface:
                    OffsetSurface += 1;
                    break;
                default:
                    unknown += 1;
                }
            }
        }
    }
    std::cout << boost::str(boost::format("\tSurfaces tally:\n\t===============\n\t- Planes: %d\n\t- Cylinders: %d\n\t- Tori: %d\n\t"
                                          "- Cones: %d\n\t- Spheres: %d\n\t- B-Splines: %d\n\t- Bezier Surfaces: %d\n\t"
                                          "- Surfaces of Revolution: %d\n\t- Surfaces of Extrusion: %d\n\t"
                                          "- Offset Surfaces: %d\n\t- Uknown Surfaces: %d") 
                            % planes % cylinders % tori % cones % spheres % BezierSurface % BSpline % SurfaceOfRevolution
                            % SurfaceOfExtrusion % OffsetSurface % unknown) << std::endl;
}
