// McCAD
#include "torusConvertor.hpp"
#include "preprocessor.hpp"
#include "solidType.hpp"
#include "ShapeView.hpp"
#include "boundSurfacePlane_impl.hpp"
#include "surfaceObjCerator.hpp"
//OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <gp_Ax2.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <STEPControl_Writer.hxx>

void
McCAD::Decomposition::TorusConvertor::operator()(Geometry::Solid::Impl& solidImpl){
    auto& solidsList = *solidImpl.splitSolidList;
    for(Standard_Integer index = 1; index <= solidsList.Length(); ++index){
        if (Preprocessor{}.determineSolidType(TopoDS::Solid(solidsList(index))) ==
                Tools::SolidType{}.toroidal){
            solidsList.InsertAfter(index, convertTorusToCylinder(solidsList(index)));
            solidsList.Remove(index);
        }
    }
}

TopoDS_Shape
McCAD::Decomposition::TorusConvertor::convertTorusToCylinder(
        const TopoDS_Shape& shape, Standard_Real scaleFactor){
    std::vector<BRepAdaptor_Surface> planesList;
    Standard_Real radius;
    for(const auto& face : detail::ShapeView<TopAbs_FACE>{shape}){
        if (BRepAdaptor_Surface(face).GetType() == GeomAbs_Plane){
            planesList.push_back(face);
        }
        else if (BRepAdaptor_Surface(face).GetType() == GeomAbs_Torus){
            radius = BRepAdaptor_Surface(face).Torus().MinorRadius();
        }
    }
    if (planesList.size() != 2) return shape;
    // create cylinder
    gp_Vec vector(planesList[0].Plane().Location(), planesList[1].Plane().Location());
    gp_Ax2 axis(planesList[0].Plane().Location(), gp_Dir(vector));
    axis.Translate(0.5*(1 - scaleFactor)*vector);
    TopoDS_Face cylinder = BRepPrimAPI_MakeCylinder(axis, radius,
                                                    scaleFactor*vector.Magnitude());
    //createSolid(cylinder, planesList);
    //debug
    STEPControl_Writer writer7;
    writer7.Transfer(cylinder, STEPControl_StepModelType::STEPControl_AsIs);
    writer7.Transfer(shape, STEPControl_StepModelType::STEPControl_AsIs);
    Standard_Integer kk = 0;
    std::string filename = "/home/mharb/Documents/McCAD_refactor/examples/bbox/torCyl";
    std::string suffix = ".stp";
    while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
        ++kk;
    }
    filename += std::to_string(kk);
    filename += suffix;
    writer7.Write(filename.c_str());
    //debug
    return shape;
}

void
McCAD::Decomposition::TorusConvertor::createSolid(
        TopoDS_Face& cylinder, std::vector<BRepAdaptor_Surface>& planesList){
    std::cout << "create solid" << std::endl;
    auto firstSurface = GeomAdaptor_Surface(planesList[0].Surface()).Surface();
    auto secondSurface = GeomAdaptor_Surface(planesList[0].Surface()).Surface();
    auto thirdSurface = GeomAdaptor_Surface(BRepAdaptor_Surface(cylinder).Surface()).Surface();
    try {BRepBuilderAPI_MakeSolid(BRepBuilderAPI_MakeShell(firstSurface).Shell(),
                                  BRepBuilderAPI_MakeShell(secondSurface).Shell(),
                                  BRepBuilderAPI_MakeShell(thirdSurface).Shell());
    } catch (...) {};
}

