// McCAD
#include "preprocessor.hpp"

McCAD::Decomposition::Preprocessor::Preprocessor(){
}

McCAD::Decomposition::Preprocessor::~Preprocessor(){
}

McCAD::Decomposition::Preprocessor::VariantType
McCAD::Decomposition::Preprocessor::perform(const TopoDS_Shape& shape){
    VariantType solidVariant;
    if (auto& solid = TopoDS::Solid(shape); !checkBndSurfaces(solid)){
        switch (determineSolidType(solid)){
        case solidType.planarSolid:{
            solidVariant = SolidObjCreator{}.createObj<
                    McCAD::Geometry::PLSolid>(shape);
            return solidVariant;
        }
        case solidType.cylindricalSolid:{
            solidVariant = SolidObjCreator{}.createObj<
                    McCAD::Geometry::CYLSolid>(shape);
            return solidVariant;
        }
        case solidType.toroidal:{
            solidVariant = SolidObjConstructor{}.constructObj<
                    McCAD::Geometry::TORSolid>(shape);
            return solidVariant;
        }
        default:
            goto rejectSolid;
        }
    }
    rejectSolid: std::cout << "   - Processing of solids with non-planar surfaces"
                              " is not yet supported!.\n     Solid will be added"
                              " to rejected solids file" << std::endl;
    return solidVariant;
}

Standard_Boolean
McCAD::Decomposition::Preprocessor::checkBndSurfaces(const TopoDS_Solid& solid){
    for(const auto& face : detail::ShapeView<TopAbs_FACE>{solid}){
        GeomAdaptor_Surface surfAdaptor(BRep_Tool::Surface(face));
        if (surfAdaptor.GetType() == GeomAbs_BSplineSurface){
            std::cout << "    -- The current verion doesn't support processing "
                         "of splines. Solid will be rejected!" << std::endl;
            return Standard_True;
        }
    }
    return Standard_False;
}

Standard_Integer
McCAD::Decomposition::Preprocessor::determineSolidType(const TopoDS_Solid& solid){
    Standard_Boolean planar, cylindrical, toroidal, spherical, mixed;
    planar = cylindrical = toroidal = spherical = mixed = Standard_False;
    for(const auto& face : detail::ShapeView<TopAbs_FACE>{solid}){
        GeomAdaptor_Surface surfAdaptor(BRep_Tool::Surface(face));
        switch (surfAdaptor.GetType()){
        case  GeomAbs_Plane:
            planar = Standard_True;
            break;
        case GeomAbs_Cylinder:
            cylindrical = Standard_True;
            break;
        case GeomAbs_Torus:
            toroidal = Standard_True;
            break;
        case GeomAbs_Sphere:
            spherical = Standard_True;
            break;
        default:
            mixed = Standard_True;
        }
    }
    // Determine solid type based on surfaces types.
    // This is to be modified as was done in SurfaceUtilities.
    if (mixed || (cylindrical && spherical) || (cylindrical && toroidal) ||
            (toroidal && spherical)) return solidType.mixed;
    else if (spherical) return solidType.spherical;
    else if (cylindrical) return solidType.cylindrical;
    else if (toroidal) return solidType.toroidal;
    else return solidType.mixed;
}
