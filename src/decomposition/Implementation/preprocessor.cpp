// McCAD
#include "preprocessor.hpp"
#include "heirarchyFlatter.hpp"
#include "solidObjCreator.hpp"

McCAD::Decomposition::Preprocessor::Preprocessor(){}

McCAD::Decomposition::Preprocessor::~Preprocessor(){}

McCAD::Decomposition::Preprocessor::Preprocessor(
        const std::unique_ptr<Geometry::Impl::Compound>& compound){
    auto product = Tools::HeirarchyFlatter{}(compound->compoundShape);
    compound->acceptedInputShapesList = std::move(product.first);
    compound->rejectedInputShapesList = std::move(product.second);
    for(const auto& shape : *compound->acceptedInputShapesList){
        auto solidObj = perform(shape);
        if (std::holds_alternative<std::monostate>(solidObj)){
            compound->rejectedInputShapesList->Append(shape);
        } else {
            // Using switch for now. Should be separated in a separate class an called
            // for each specific type of solid object.
            switch (Standard_Integer(solidObj.index())){
            case solidType.planar:
                compound->planarSolidsList.push_back(std::get<solidType.planar>(solidObj));
                break;
            case solidType.cylindrical:
                compound->cylSolidsList.push_back(std::get<solidType.cylindrical>(solidObj));
                break;
            case solidType.toroidal:
                compound->torSolidsList.push_back(std::get<solidType.toroidal>(solidObj));
                break;
            default:
                std::cout << "   - Processing of solids with non-planar surfaces"
                             " is not yet supported!.\n     Solid will be added"
                             " to rejected solids file" << std::endl;
                compound->rejectedInputShapesList->Append(shape);
            }
        }
    }
}

McCAD::Decomposition::Preprocessor::VariantType
McCAD::Decomposition::Preprocessor::perform(const TopoDS_Shape& shape){
    VariantType solidVariant;
    if (auto& solid = TopoDS::Solid(shape); !checkBndSurfaces(solid)){
        switch (determineSolidType(solid)){
        case solidType.planar:
            solidVariant = SolidObjCreator{}.createObj<Geometry::PLSolid>(shape);
            break;
        case solidType.cylindrical:
            solidVariant = SolidObjCreator{}.createObj<Geometry::CYLSolid>(shape);
            break;
        case solidType.toroidal:
            solidVariant = SolidObjCreator{}.createObj<Geometry::TORSolid>(shape);
            break;
        default:;
            // Unknown Type
        }
    }
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
    Standard_Boolean planar{Standard_False}, cylindrical{Standard_False},
                     toroidal{Standard_False}, spherical{Standard_False},
                     mixed{Standard_False};
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
    // Determine custom solid type based on surfaces types.
    if (mixed || (cylindrical && spherical) || (cylindrical && toroidal) ||
            (toroidal && spherical) || (cylindrical && spherical && toroidal))
        return solidType.mixed;
    else if (cylindrical) return solidType.cylindrical;
    else if (toroidal) return solidType.toroidal;
    else if (spherical) return solidType.spherical;
    else if (planar) return solidType.planar;
    else return solidType.unKnown;
}
