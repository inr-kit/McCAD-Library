// C++
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
// McCAD
#include "preprocessor.hpp"
#include "heirarchyFlatter.hpp"
#include "solidObjCreator.hpp"
#include "ShapeView.hpp"
#include "ShapeUtilities.hpp"
// OCCT
#include <TopoDS_Face.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>

McCAD::Decomposition::Preprocessor::Preprocessor(){}

McCAD::Decomposition::Preprocessor::Preprocessor(const IO::InputConfig& inputConfig) :
    inputConfig{inputConfig}{}

McCAD::Decomposition::Preprocessor::~Preprocessor(){}

/** ********************************************************************
* @brief    The main function that checks the volume and boundary surfaces and assigns solid type.
* @param    compound is a McCAD compound object containing a shape and name.
* @date     31/12/2020
* @modified 22/08/2022
* @author   Moataz Harb
* **********************************************************************/
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
            switch (int(solidObj.index())){
            case solidType.planar:
                compound->planarSolidsList.push_back(std::get<solidType.planar>(solidObj));
                break;
            case solidType.cylindrical:
                compound->cylSolidsList.push_back(std::get<solidType.cylindrical>(solidObj));
                break;
            case solidType.toroidal:
                compound->torSolidsList.push_back(std::get<solidType.toroidal>(solidObj));
                break;
            case solidType.mixed:
                compound->mixedSolidsList.push_back(std::get<solidType.mixed>(solidObj));
                break;
            case solidType.conical:
                compound->conSolidsList.push_back(std::get<solidType.conical>(solidObj));
                break;
            default:;
            }
        }
    }
    // Add solids to solids list. Planar, then cylindrical, then toroidal, then mixed, then conical.
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
    if(compound->mixedSolidsList.size() > 0)
        compound->solidsList.insert(compound->solidsList.end(),
                                    compound->mixedSolidsList.begin(),
                                    compound->mixedSolidsList.end());
    if(compound->conSolidsList.size() > 0)
        compound->solidsList.insert(compound->solidsList.end(),
                                    compound->conSolidsList.begin(),
                                    compound->conSolidsList.end());
}

/** ********************************************************************
* @brief    A function that determines the solid type and creates a corresponding solid object.
* @param    shape is a OCCT shape.
* @return   variant.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
McCAD::Decomposition::Preprocessor::VariantType
McCAD::Decomposition::Preprocessor::perform(const TopoDS_Shape& shape){
    VariantType solidVariant;
    switch (determineSolidType(TopoDS::Solid(shape))){
    case solidType.planar:
        solidVariant = SolidObjCreator{inputConfig}.createObj<Geometry::PLSolid>(shape);
        break;
    case solidType.cylindrical:
        solidVariant = SolidObjCreator{inputConfig}.createObj<Geometry::CYLSolid>(shape);
        break;
    case solidType.toroidal:
        solidVariant = SolidObjCreator{inputConfig}.createObj<Geometry::TORSolid>(shape);
        break;
    case solidType.mixed:
        solidVariant = SolidObjCreator{inputConfig}.createObj<Geometry::MXDSolid>(shape);
        break;
    case solidType.conical:
        solidVariant = SolidObjCreator{ inputConfig }.createObj<Geometry::CONSolid>(shape);
        break;
    default:;
        // Unknown Type. Returns an empty variant.
    }
    return solidVariant;
}

/** ********************************************************************
* @brief    A function that checks if the volume of a solid contains unsupported surfaces.
* @param    shape is a OCCT shape.
* @return   bool. True if the surface is not supported.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::Preprocessor::checkBndSurfaces(const TopoDS_Shape& shape){
    for(const auto& face : detail::ShapeView<TopAbs_FACE>{TopoDS::Solid(shape)}){
        GeomAdaptor_Surface surfAdaptor(BRep_Tool::Surface(face));
        if (!Tools::checkSupported(surfAdaptor.GetType())){
            std::cout << boost::str(boost::format("    -- The current verion doesn't support processing "
                                                  "of surfaces of type %s. Solid will be rejected!") 
                                    % Tools::toTypeName(surfAdaptor.GetType())) << std::endl;
            return true;
        }
    }
    return false;
}

/** ********************************************************************
* @brief    A function that checks if the volume of a solid is less than a limit.
* @param    shape is a OCCT shape.
* @return   bool. True if the solid volume is below the limit.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::Preprocessor::checkVolume(const TopoDS_Shape& shape){
    double shapeVolume{ Tools::calcVolume(shape) };
    if (shapeVolume < inputConfig.minSolidVolume) return true;
    else return false;
}

/** ********************************************************************
* @brief    A function that determines the solid type.
* @param    solid is a OCCT solid.
* @return   variant.
* @date     31/12/2020
* @modified 22/08/2022
* @author   Moataz Harb
* **********************************************************************/
int
McCAD::Decomposition::Preprocessor::determineSolidType(const TopoDS_Solid& solid){
    bool planar{false}, cylindrical{false}, toroidal{false}, mixed{false}, 
         conical{false}, spherical{false};
    for(const auto& face : detail::ShapeView<TopAbs_FACE>{solid}){
        GeomAdaptor_Surface surfAdaptor(BRep_Tool::Surface(face));
        switch (surfAdaptor.GetType()){
        case  GeomAbs_Plane:
            planar = true;
            break;
        case GeomAbs_Cylinder:
            cylindrical = true;
            break;
        case GeomAbs_Torus:
            toroidal = true;
            break;
        case GeomAbs_Cone:
            conical = true;
            break;
        case GeomAbs_Sphere:
            spherical = true;
            break;
        default:
            mixed = true;
        }
    }
    // Determine custom solid type based on surfaces types. If the solid contains a
    // unique surface type, besides planar, then it is labeled according to that surface.
    // If any mix of surfaces exists, asde from planes, label the solid as mixed.
    if (mixed || (cylindrical && toroidal) || (cylindrical && conical) ||
       (cylindrical && spherical) || (toroidal && conical) || (toroidal && spherical) ||
       (conical && spherical))
        return solidType.mixed;
    // The solid is either a mix a planar and another type or pure planar.
    else if (spherical) return solidType.spherical;
    else if (conical) return solidType.conical;
    else if (toroidal) return solidType.toroidal;
    else if (cylindrical) return solidType.cylindrical;
    else if (planar) return solidType.planar;
    else return solidType.unKnown;
}
