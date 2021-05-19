// C++
#include <set>
// McCAD
#include "torusConvertor.hpp"
#include "preprocessor.hpp"
#include "SolidType.hpp"
#include "ShapeView.hpp"
#include "solid_impl.hpp"
#include "boundSurfacePlane_impl.hpp"
#include "surfaceObjCerator.hpp"
#include "pointInSolid.hpp"
#include "SolidSplitter.hpp"
//OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <gp_Ax2.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>
#include <gp_Torus.hxx>
#include <BRepBndLib.hxx>
#include <Bnd_OBB.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepAdaptor_Surface.hxx>

void
McCAD::Decomposition::TorusConvertor::operator()(
        const std::shared_ptr<Geometry::Solid>& solid,
        const Standard_Real& aScalingFactor){
    scalingFactor = aScalingFactor;
    auto& solidsList = *solid->accessSImpl()->splitSolidList;
    for(Standard_Integer index = 1; index <= solidsList.Length(); ++index){
        //std::cout << "TorusConvertor" << std::endl;
        if (Preprocessor{}.determineSolidType(TopoDS::Solid(solidsList(index))) ==
                Tools::SolidType{}.toroidal){
            auto newShape = convertTorusToCylinder(solidsList(index));
            if (!newShape.has_value()) continue;
            solidsList.InsertAfter(index, *newShape);
            solidsList.Remove(index);
        }
    }
}

std::optional<TopoDS_Shape>
McCAD::Decomposition::TorusConvertor::convertTorusToCylinder(
        const TopoDS_Shape& shape, Standard_Real scaleFactor){
    //std::cout << "convertTorusToCylinder" << std::endl;
    std::vector<TopoDS_Face> planesList;
    std::set<Standard_Real> radii;
    gp_Dir torDir;
    //gp_Ax3 torCoor, cylCoor;
    for(const auto& face : detail::ShapeView<TopAbs_FACE>{shape}){
        if (BRepAdaptor_Surface(face).GetType() == GeomAbs_Plane){
            planesList.push_back(face);
        }
        else if (BRepAdaptor_Surface(face).GetType() == GeomAbs_Torus){
            radii.insert(BRepAdaptor_Surface(face).Torus().MinorRadius());
            torDir = BRepAdaptor_Surface(face).Torus().Axis().Direction();
            //torCoor = BRepAdaptor_Surface(face).Torus().Position();
        }
    }
    if (planesList.size() != 2 || radii.size() > 2) return shape;
    // Create cylinder
    gp_Vec vector(BRepAdaptor_Surface(planesList[0]).Plane().Location(),
            BRepAdaptor_Surface(planesList[1]).Plane().Location());
    if (!PointInSolid{}(shape, BRepAdaptor_Surface(planesList[0]).Plane().Location()) ||
            !PointInSolid{}(shape, BRepAdaptor_Surface(planesList[1]).Plane().Location())){
        std::cout << "***** outside *******" << std::endl;
        return shape;
    }
    //if (gp_Dir(vector).Angle(torDir) != 1.5708) return shape;
    gp_Ax2 axis(BRepAdaptor_Surface(planesList[0]).Plane().Location(), gp_Dir(vector));
    axis.Translate(0.5*(1 - scaleFactor)*vector);
    TopoDS_Solid cylinder = BRepPrimAPI_MakeCylinder(axis, *radii.rbegin(),
                                                     scaleFactor*vector.Magnitude());
    /*
    for(const auto& face : detail::ShapeView<TopAbs_FACE>{cylinder}){
        if (BRepAdaptor_Surface(face).GetType() == GeomAbs_Cylinder){
            std::cout << "cyl axis: " << BRepAdaptor_Surface(face).Cylinder().Axis().Direction().X() <<
                         ", " << BRepAdaptor_Surface(face).Cylinder().Axis().Direction().Y() <<
                         ", " << BRepAdaptor_Surface(face).Cylinder().Axis().Direction().Z() << std::endl;
            std::cout << "cyl axis angle w vecotr: " << BRepAdaptor_Surface(face).Cylinder().Axis().Direction().Angle(gp_Dir(vector)) << std::endl;
            std::cout << "cyl axis angle w torAxis: " << BRepAdaptor_Surface(face).Cylinder().Axis().Direction().Angle(torDir) << std::endl;
            cylCoor = BRepAdaptor_Surface(face).Cylinder().Position();
        }}
    std::cout << "Coord: " << torCoor.Angle(cylCoor) << std::endl;
    STEPControl_Writer writer0;
    writer0.Transfer(shape, STEPControl_StepModelType::STEPControl_AsIs);
    writer0.Transfer(cylinder, STEPControl_StepModelType::STEPControl_AsIs);*/
    if (radii.size() == 2) retrieveSolid(cylinder, planesList, *(++radii.rbegin()),
                                         scaleFactor);
    /*
    for(const auto& face : detail::ShapeView<TopAbs_FACE>{cylinder}){
        if (BRepAdaptor_Surface(face).GetType() == GeomAbs_Cylinder){
            std::cout << "cyl axis: " << BRepAdaptor_Surface(face).Cylinder().Axis().Direction().X() <<
                         ", " << BRepAdaptor_Surface(face).Cylinder().Axis().Direction().Y() <<
                         ", " << BRepAdaptor_Surface(face).Cylinder().Axis().Direction().Z() << std::endl;
            std::cout << "cyl symmetry: " << BRepAdaptor_Surface(face).Cylinder().Axis().Direction().Angle(gp_Dir(vector)) << std::endl;
            std::cout << "cyl symmetry: " << BRepAdaptor_Surface(face).Cylinder().Axis().Direction().Angle(torDir) << std::endl;
            cylCoor = BRepAdaptor_Surface(face).Cylinder().Position();
        }}
    gp_Trsf transformation;
    transformation.SetTransformation(cylCoor, torCoor);
    BRepBuilderAPI_Transform cylTransform{transformation};
    cylTransform.Perform(cylinder);
    for (const auto& solid : detail::ShapeView<TopAbs_SOLID>{cylTransform.ModifiedShape(cylinder)}){
        cylinder = solid;
    }*/
    auto newSolid = fitCylinder(cylinder, planesList);
    /*//debug
    writer0.Transfer(cylinder, STEPControl_StepModelType::STEPControl_AsIs);
    Standard_Integer kk = 0;
    std::string filename = "/home/mharb/Documents/McCAD_refactor/examples/bbox/cyl";
    std::string suffix = ".stp";
    while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
        ++kk;
    }
    filename += std::to_string(kk);
    filename += suffix;
    writer0.Write(filename.c_str());
    *///debug
    if(!newSolid.has_value()) return shape;
    return *newSolid;
}

void
McCAD::Decomposition::TorusConvertor::retrieveSolid(
        TopoDS_Solid& cylinder, const std::vector<TopoDS_Face>& planesList,
        Standard_Real innerRadius, const Standard_Real& scaleFactor){
    //std::cout << "retrieveSolid" << std::endl;
    // If the original torus solid was hollow the created cylinder also shoudld be.
    gp_Vec vector(BRepAdaptor_Surface(planesList[0]).Plane().Location(),
            BRepAdaptor_Surface(planesList[1]).Plane().Location());
    gp_Ax2 axis(BRepAdaptor_Surface(planesList[0]).Plane().Location(),
            gp_Dir(vector));
    axis.Translate(0.5*(1 - 2*scaleFactor)*vector);
    TopoDS_Solid innerCylinder = BRepPrimAPI_MakeCylinder(axis,
                                                          innerRadius,
                                                          2*scaleFactor*vector.Magnitude());
    BRepAlgoAPI_Cut cutter{cylinder, innerCylinder};
    if (cutter.IsDone()){
        for (const auto& solid : detail::ShapeView<TopAbs_SOLID>{cutter.Shape()}){
            cylinder = solid;
            return;
            }
        }
}

std::optional<TopoDS_Shape>
McCAD::Decomposition::TorusConvertor::fitCylinder(
        TopoDS_Solid& cylinder, std::vector<TopoDS_Face>& planesList){
    //std::cout << "fitCylinder" << std::endl;
    // Split extra part on one side of the cylinder
    auto firstSolids = splitSolid(cylinder, planesList[0]);
    if(!firstSolids.has_value()) return std::nullopt;
    // Split extra part on the other side of cylinder
    if (PointInSolid{}(firstSolids->first,
                       BRepAdaptor_Surface(planesList[1]).Plane().Location())){
        TopoDS_Solid solid;
        for(const auto& aSolid : detail::ShapeView<TopAbs_SOLID>{firstSolids->first}){
            solid = aSolid;
        }
        auto secondSolids = splitSolid(solid, planesList[1]);
        if(!secondSolids.has_value()) return std::nullopt;
        else if (PointInSolid{}(secondSolids->first,
                                BRepAdaptor_Surface(planesList[0]).Plane().Location())){
            return secondSolids->first;
        } else return secondSolids->second;
    } else {
        TopoDS_Solid solid;
        for(const auto& aSolid : detail::ShapeView<TopAbs_SOLID>{firstSolids->second}){
            solid = aSolid;
        }
        auto secondSolids = splitSolid(solid, planesList[1]);
        if(!secondSolids.has_value()) return std::nullopt;
        else if (PointInSolid{}(secondSolids->first,
                                BRepAdaptor_Surface(planesList[0]).Plane().Location())){
            return secondSolids->first;
        } else return secondSolids->second;
    }
}

std::optional<std::pair<TopoDS_Shape, TopoDS_Shape>>
McCAD::Decomposition::TorusConvertor::splitSolid(TopoDS_Solid& solid,
                                                 TopoDS_Face& splitFace){
    //std::cout << "splitSolid" << std::endl;
    auto solidObj = Geometry::Solid::Impl{};
    solidObj.initiate(solid);
    solidObj.createOBB();
    solidObj.calcMeshDeflection(scalingFactor);
    auto extFace = SurfaceObjCreator{}(splitFace, solidObj.boxDiagonalLength, 1.0e-7);
    auto resultSolids = SolidSplitter{}(solid, solidObj.obb,
                                        extFace->accessSImpl()->extendedFace);
    return resultSolids;
}
