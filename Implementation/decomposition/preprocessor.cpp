// McCAD
#include "preprocessor.hpp"

McCAD::Decomposition::Preprocessor::Preprocessor(){
}

McCAD::Decomposition::Preprocessor::~Preprocessor(){
}

McCAD::Decomposition::Preprocessor::variantType
McCAD::Decomposition::Preprocessor::perform(const TopoDS_Shape& shape){
    Preprocessor::variantType solidVariant;
    auto& solid = TopoDS::Solid(shape);
    if (!checkBndSurfaces(solid)){
        switch (determineSolidType(solid)){
        case solidType.planarSolid:{
            solidVariant = SolidObjConstructor{}.constructObj<
                    McCAD::Geometry::PLSolid>(shape);
            return solidVariant;
        }
        case solidType.cylindricalSolid:{
            solidVariant = SolidObjConstructor{}.constructObj<
                    McCAD::Geometry::CYLSolid>(shape);
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
        if(surfAdaptor.GetType() == GeomAbs_Torus){
            std::cout << "    -- The current verion doesn't support processing "
                         "of Tori. Solid will be rejected!" << std::endl;
            return Standard_True;
        } else if (surfAdaptor.GetType() == GeomAbs_BSplineSurface){
            std::cout << "    -- The current verion doesn't support processing "
                         "of splines. Solid will be rejected!" << std::endl;
            return Standard_True;
        }
    }
    return Standard_False;
}

Standard_Integer
McCAD::Decomposition::Preprocessor::determineSolidType(const TopoDS_Solid& solid){
    Standard_Boolean plSolid, cylSolid, sphSolid, mxdSolid;
    cylSolid = sphSolid = plSolid = mxdSolid = Standard_False;
    for(const auto& face : detail::ShapeView<TopAbs_FACE>{solid}){
        GeomAdaptor_Surface surfAdaptor(BRep_Tool::Surface(face));
        switch (surfAdaptor.GetType()){
        case  GeomAbs_Plane:
            plSolid = Standard_True;
            break;
        case GeomAbs_Cylinder:
            cylSolid = Standard_True;
            break;
        case GeomAbs_Sphere:
            sphSolid = Standard_True;
            break;
        default:
            mxdSolid = Standard_True;
        }
    }
    // Determine solid type based on surfaces types.
    // This is to be modified as was done in SurfaceUtilities.
    if (mxdSolid || (cylSolid && sphSolid)) return solidType.mixedSolid;
    else if (sphSolid) return solidType.sphericalSolid;
    else if (cylSolid) return solidType.cylindricalSolid;
    else return solidType.planarSolid;
}
