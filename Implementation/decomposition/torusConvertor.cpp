// McCAD
#include "torusConvertor.hpp"
#include "preprocessor.hpp"
#include "solidType.hpp"
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
#include <BRepBndLib.hxx>
#include <Bnd_OBB.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <gp_Circ.hxx>
#include <BRepAlgoAPI_Cut.hxx>

void
McCAD::Decomposition::TorusConvertor::operator()(Geometry::Solid::Impl& solidImpl){
    auto& solidsList = *solidImpl.splitSolidList;
    for(Standard_Integer index = 1; index <= solidsList.Length(); ++index){
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
    std::vector<TopoDS_Face> planesList;
    Standard_Real radius, inner_radius{0};
    for(const auto& face : detail::ShapeView<TopAbs_FACE>{shape}){
        if (BRepAdaptor_Surface(face).GetType() == GeomAbs_Plane){
            planesList.push_back(face);
        }
        else if (BRepAdaptor_Surface(face).GetType() == GeomAbs_Torus){
            radius = BRepAdaptor_Surface(face).Torus().MinorRadius();
        }
    }
    for(const auto& edge : detail::ShapeView<TopAbs_EDGE>{shape}){
        if (Tools::toTypeName(BRepAdaptor_Curve(edge).GetType()) == "Circle" &&
            BRepAdaptor_Curve(edge).Circle().Radius() < radius){
                inner_radius = BRepAdaptor_Curve(edge).Circle().Radius();
            }
    }
    if (planesList.size() != 2) return shape;
    // Create cylinder
    gp_Vec vector(BRepAdaptor_Surface(planesList[0]).Plane().Location(),
            BRepAdaptor_Surface(planesList[1]).Plane().Location());
    gp_Ax2 axis(BRepAdaptor_Surface(planesList[0]).Plane().Location(), gp_Dir(vector));
    axis.Translate(0.5*(1 - scaleFactor)*vector);
    TopoDS_Solid cylinder = BRepPrimAPI_MakeCylinder(axis, radius, scaleFactor*vector.Magnitude());
    if (inner_radius > 0) retrieveSolid(cylinder, planesList, inner_radius, scaleFactor);
    auto newSolid = fitCylinder(cylinder, planesList);
    if(!newSolid.has_value()) return shape;
    return *newSolid;
}

void
McCAD::Decomposition::TorusConvertor::retrieveSolid(
        TopoDS_Solid& cylinder, const std::vector<TopoDS_Face>& planesList,
        Standard_Real innerRadius, const Standard_Real& scaleFactor){
    // If the original torus solid was hollow the created cylinder also shoudld be.
    gp_Vec vector(BRepAdaptor_Surface(planesList[0]).Plane().Location(),
            BRepAdaptor_Surface(planesList[1]).Plane().Location());
    gp_Ax2 axis(BRepAdaptor_Surface(planesList[0]).Plane().Location(),
            gp_Dir(vector));
    axis.Translate(0.5*(1 - 1.5*scaleFactor)*vector);
    TopoDS_Solid innerCylinder = BRepPrimAPI_MakeCylinder(axis,
                                                          innerRadius,
                                                          1.5*scaleFactor*vector.Magnitude());
    std::cout << "cutting" << std::endl;
    BRepAlgoAPI_Cut cutter{cylinder, innerCylinder};
    if (cutter.IsDone()){
        std::cout << "casting" << std::endl;
        for (const auto& solid : detail::ShapeView<TopAbs_SOLID>{cutter.Shape()}){
            cylinder = solid;
            return;
            }
        }
}

std::optional<TopoDS_Shape>
McCAD::Decomposition::TorusConvertor::fitCylinder(
        TopoDS_Solid& cylinder, std::vector<TopoDS_Face>& planesList){
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
    auto solidObj = Geometry::Solid::Impl{};
    solidObj.initiate(solid);
    solidObj.createOBB();
    solidObj.calcMeshDeflection();
    auto extFace = SurfaceObjCreator{}(splitFace, solidObj.boxDiagonalLength);
    auto resultSolids = SolidSplitter{}(solid, solidObj.obb,
                                        extFace->accessSImpl()->extendedFace);
    return resultSolids;
}
