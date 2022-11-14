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
#include <STEPControl_Writer.hxx>

McCAD::Decomposition::TorusConvertor::TorusConvertor(
        const IO::InputConfig& inputConfig) : inputConfig{inputConfig}{}

McCAD::Decomposition::TorusConvertor::~TorusConvertor(){}

void
McCAD::Decomposition::TorusConvertor::operator()(
        const std::shared_ptr<Geometry::Solid>& solid){
    auto& solidsList = *solid->accessSImpl()->splitSolidList;
    for(Standard_Integer index = 1; index <= solidsList.Length(); ++index){
        if (Preprocessor{}.determineSolidType(TopoDS::Solid(solidsList(index))) ==
                Tools::SolidType{}.toroidal){
            auto newShape = convertTorusToCylinder(solidsList(index));
            if (!newShape.has_value()) continue;
            // Check whether the simplificstion was successful or not.
            solidsList.InsertAfter(index, *newShape);
            solidsList.Remove(index);
        }
    }
}

Standard_Boolean
McCAD::Decomposition::TorusConvertor::convertCondition(const TopoDS_Shape& shape){
    // Check whether to simplify all tori or just the non-coaxial.
    if(inputConfig.simplifyTori){
        if (inputConfig.simplifyAllTori) return Standard_True;
        else{
            // Simplify only non-coaxial tori.
            for(const auto& face : detail::ShapeView<TopAbs_FACE>{shape}){
                if (BRepAdaptor_Surface(face).GetType() == GeomAbs_Torus){
                    gp_Dir torDir = BRepAdaptor_Surface(face).Torus().Axis().Direction();
                    if ((std::abs(torDir.X()) < inputConfig.precision &&
                         std::abs(torDir.Y()) < inputConfig.precision) ||
                        (std::abs(torDir.X()) < inputConfig.precision &&
                         std::abs(torDir.Z()) < inputConfig.precision) ||
                        (std::abs(torDir.Y()) < inputConfig.precision &&
                         std::abs(torDir.Z()) < inputConfig.precision))
                        return Standard_False;
                    else return Standard_True;
                }
            }
        }
    } else return Standard_False;
}

std::optional<TopoDS_Shape>
McCAD::Decomposition::TorusConvertor::convertTorusToCylinder(const TopoDS_Shape& shape){
    if(!convertCondition(shape)) return std::nullopt;
    std::vector<TopoDS_Face> planesList;
    std::set<Standard_Real> radii;
    gp_Dir torDir;
    for(const auto& face : detail::ShapeView<TopAbs_FACE>{shape}){
        if (BRepAdaptor_Surface(face).GetType() == GeomAbs_Plane){
            planesList.push_back(face);
        }
        else if (BRepAdaptor_Surface(face).GetType() == GeomAbs_Torus){
            radii.insert(BRepAdaptor_Surface(face).Torus().MinorRadius());
            torDir = BRepAdaptor_Surface(face).Torus().Axis().Direction();
        }
    }
    if (planesList.size() != 2 || radii.size() > 2) return std::nullopt;
    // Create cylinder
    gp_Pnt vecStart{BRepAdaptor_Surface(planesList[0]).Plane().Location()},
           vecEnd{BRepAdaptor_Surface(planesList[1]).Plane().Location()};
    gp_Vec vector(vecStart, vecEnd);
    if (!PointInSolid{}(shape, vecStart) || !PointInSolid{}(shape, vecEnd)){
        return std::nullopt;
    }
    gp_Ax2 axis(vecStart.Translated(-1*vector.Magnitude()*gp_Dir(vector)), gp_Dir(vector));
    //axis.Translate(0.5*(1 - scaleFactor)*vector);
    TopoDS_Solid cylinder = BRepPrimAPI_MakeCylinder(axis, *radii.rbegin(),
                                                     3*vector.Magnitude());
    /*
    for(const auto& face : detail::ShapeView<TopAbs_FACE>{cylinder}){
        if (BRepAdaptor_Surface(face).GetType() == GeomAbs_Cylinder){
            std::cout << "cyl axis: " << BRepAdaptor_Surface(face).Cylinder().Axis().Direction().X() <<
                         ", " << BRepAdaptor_Surface(face).Cylinder().Axis().Direction().Y() <<
                         ", " << BRepAdaptor_Surface(face).Cylinder().Axis().Direction().Z() << std::endl;
            std::cout << "cyl axis angle w vecotr: " << BRepAdaptor_Surface(face).Cylinder().Axis().Direction().Angle(gp_Dir(vector)) << std::endl;
            std::cout << "cyl axis angle w torAxis: " << BRepAdaptor_Surface(face).Cylinder().Axis().Direction().Angle(torDir) << std::endl;
            std::cout << "------------" << std::endl;
        }}
    STEPControl_Writer writer0;
    writer0.Transfer(shape, STEPControl_StepModelType::STEPControl_AsIs);
    writer0.Transfer(cylinder, STEPControl_StepModelType::STEPControl_AsIs);
    if(!std::filesystem::exists("cyl.stp")) writer0.Write("cyl.stp");
    */
    if (radii.size() == 2) retrieveSolid(cylinder, planesList, *(++radii.rbegin()));
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
    return *newSolid;
}

void
McCAD::Decomposition::TorusConvertor::retrieveSolid(
        TopoDS_Solid& cylinder, const std::vector<TopoDS_Face>& planesList,
        Standard_Real innerRadius){
    // If the original torus solid was hollow the created cylinder also shoudld be.
    gp_Pnt vecStart{BRepAdaptor_Surface(planesList[0]).Plane().Location()},
           vecEnd{BRepAdaptor_Surface(planesList[1]).Plane().Location()};
    gp_Vec vector(vecStart, vecEnd);
    gp_Ax2 axis(vecStart.Translated(-1*vector.Magnitude()*gp_Dir(vector)), gp_Dir(vector));
    //axis.Translate(0.5*(1 - 2*scaleFactor)*vector);
    TopoDS_Solid innerCylinder = BRepPrimAPI_MakeCylinder(axis,
                                                          innerRadius,
                                                          3*vector.Magnitude());
    BRepAlgoAPI_Cut cutter{cylinder, innerCylinder};
    if (cutter.IsDone()){
        for (const auto& solid : detail::ShapeView<TopAbs_SOLID>{cutter.Shape()}){
            if (Preprocessor{}.determineSolidType(TopoDS::Solid(solid)) ==
                    Tools::SolidType{}.cylindrical){
                cylinder = solid;
                return;
            }
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
    solidObj.createBB();
    solidObj.calcMeshDeflection(inputConfig.scalingFactor);
    auto extFace = SurfaceObjCreator{}(splitFace, solidObj.boxDiagonalLength, inputConfig.edgeTolerance);
    auto resultSolids = SolidSplitter{ inputConfig.debugLevel}(solid, solidObj.obb,
                                        extFace->accessSImpl()->extendedFace);
    return resultSolids;
}
