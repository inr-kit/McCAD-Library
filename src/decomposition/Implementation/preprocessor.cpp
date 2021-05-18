// McCAD
#include "preprocessor.hpp"
#include "heirarchyFlatter.hpp"
#include "solidObjCreator.hpp"
#include "ShapeView.hpp"
// OCC
#include <TopoDS_Face.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>

McCAD::Decomposition::Preprocessor::Preprocessor(){}

McCAD::Decomposition::Preprocessor::Preprocessor(const Standard_Real& minSolidVolume,
                                                 const Standard_Real& scalingFactor,
                                                 const Standard_Real& angularTolerance) :
    minSolidVolume{minSolidVolume}, scalingFactor{scalingFactor},
    angularTolerance{angularTolerance}{}

McCAD::Decomposition::Preprocessor::~Preprocessor(){}

void
McCAD::Decomposition::Preprocessor::operator()(
        const std::shared_ptr<Geometry::Impl::Compound>& compound){
    auto product = Tools::HeirarchyFlatter{}(compound->compoundShape);
    compound->rejectedInputShapesList = std::move(product.second);
    for (const auto& shape : *product.first){
        if(!checkVolume(shape) && !checkBndSurfaces(shape))
            compound->acceptedInputShapesList->Append(shape);
        else compound->rejectedInputShapesList->Append(shape);
    }
    for(const auto& shape : *compound->acceptedInputShapesList){
        VariantType solidObj = perform(shape);
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
            default:;
            }
        }
    }
    // Add solids to solids list. Planar, then cylindrical, then toroidal.
    if(compound->planarSolidsList.size() > 0)
        compound->solidsList.insert(compound->solidsList.end(),
                                    compound->planarSolidsList.begin(),
                                    compound->planarSolidsList.end());
    if(compound->cylSolidsList.size() > 0)
        compound->solidsList.insert(compound->solidsList.end(),
                                    compound->cylSolidsList.begin(),
                                    compound->cylSolidsList.end());
    if(compound->torSolidsList.size() > 0)
        compound->solidsList.insert(compound->solidsList.end(),
                                    compound->torSolidsList.begin(),
                                    compound->torSolidsList.end());
}

McCAD::Decomposition::Preprocessor::VariantType
McCAD::Decomposition::Preprocessor::perform(const TopoDS_Shape& shape){
    VariantType solidVariant;
    switch (determineSolidType(TopoDS::Solid(shape))){
    case solidType.planar:
        solidVariant = SolidObjCreator{}.createObj<Geometry::PLSolid>(shape,
                                                                      scalingFactor,
                                                                      angularTolerance);
        break;
    case solidType.cylindrical:
        solidVariant = SolidObjCreator{}.createObj<Geometry::CYLSolid>(shape,
                                                                       scalingFactor,
                                                                       angularTolerance);
        break;
    case solidType.toroidal:
        solidVariant = SolidObjCreator{}.createObj<Geometry::TORSolid>(shape,
                                                                       scalingFactor,
                                                                       angularTolerance);
        break;
    default:;
        // Unknown Type
    }
    return solidVariant;
}

Standard_Boolean
McCAD::Decomposition::Preprocessor::checkBndSurfaces(const TopoDS_Shape& shape){
    for(const auto& face : detail::ShapeView<TopAbs_FACE>{TopoDS::Solid(shape)}){
        GeomAdaptor_Surface surfAdaptor(BRep_Tool::Surface(face));
        if (surfAdaptor.GetType() == GeomAbs_BSplineSurface){
            std::cout << "    -- The current verion doesn't support processing "
                         "of splines. Solid will be rejected!" << std::endl;
            return Standard_True;
        }
    }
    return Standard_False;
}

Standard_Boolean
McCAD::Decomposition::Preprocessor::checkVolume(const TopoDS_Shape& shape){
    GProp_GProps geometryProperties;
    BRepGProp::VolumeProperties(TopoDS::Solid(shape), geometryProperties);
    if (geometryProperties.Mass() < minSolidVolume) return Standard_True;
    else return Standard_False;
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
