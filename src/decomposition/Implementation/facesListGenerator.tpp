//C++
#include <typeinfo>
//McCAD
#include "facesListGenerator.hpp"
#include "planarSolid_impl.hpp"
#include "cylSolid_impl.hpp"
#include "torSolid_impl.hpp"
#include "ShapeView.hpp"
#include "tools_impl.hpp"
#include "surfaceObjCerator.hpp"
#include "surfacesMerger.hpp"
#include "SurfaceUtilities.hpp"
//OCC
#include <TopoDS_Face.hxx>
#include <TopoDS_Solid.hxx>
#include <BRepTools.hxx>

template <typename solidObjType>
std::vector<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::FacesListGenerator::operator()(solidObjType& solidObj){
    TopoDS_Solid solid = solidObj.accessSImpl()->solid;
    TopoDS_Face face;
    Standard_Integer faceNumber = 0;
    for(const auto& aFace : detail::ShapeView<TopAbs_FACE>{solid}){
        face = aFace;
        BRepTools::Update(face);
        Standard_Boolean rejectCondition =
                Tools::Preprocessor{}.accessImpl()->checkFace(face);
        if (!rejectCondition){
            ++faceNumber;
            Tools::Preprocessor{}.accessImpl()->fixFace(face);
            std::shared_ptr<Geometry::BoundSurface> boundSurface =
                    SurfaceObjCreator{}(face,
                                        solidObj.accessSImpl()->boxDiagonalLength);
            boundSurface->accessSImpl()->surfaceNumber = faceNumber;
            if (boundSurface->accessBSImpl()->generateMesh(
                        solidObj.accessSImpl()->meshDeflection)){
                boundSurface->accessBSImpl()->generateEdges();
                if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Plane)){
                    planesList.push_back(std::move(boundSurface));
                } else if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Cylinder)){
                    cylindersList.push_back(std::move(boundSurface));
                } else if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Torus)){
                    toriList.push_back(std::move(boundSurface));
                }
            } else continue;
        }
    }
    addListsToSolidObj(solidObj);
    return facesList;
}

template <typename solidObjType>
void
McCAD::Decomposition::FacesListGenerator::addListsToSolidObj(solidObjType& solidObj){
    if (typeid(solidObjType) == typeid(McCAD::Geometry::PLSolid)){
        mergePlanesList(solidObj.accessSImpl()->boxDiagonalLength);
        solidObj.accessSImpl()->planesList = planesList;
    } else if (typeid(solidObjType) == typeid(McCAD::Geometry::CYLSolid)){
        mergePlanesList(solidObj.accessSImpl()->boxDiagonalLength);
        mergeCylindersList(solidObj.accessSImpl()->boxDiagonalLength);
        solidObj.accessSImpl()->planesList = planesList;
        solidObj.accessSImpl()->cylindersList = cylindersList;
    } else if (typeid(solidObjType) == typeid(McCAD::Geometry::TORSolid)){
        mergePlanesList(solidObj.accessSImpl()->boxDiagonalLength);
        mergeToriList(solidObj.accessSImpl()->boxDiagonalLength);
        solidObj.accessSImpl()->planesList = planesList;
        solidObj.accessSImpl()->toriList = toriList;
    } else return;
}

void
McCAD::Decomposition::FacesListGenerator::mergePlanesList(Standard_Real& boxDiagonalLength){
    SurfacesMerger{}(planesList, boxDiagonalLength);
    facesList.insert(facesList.end(), planesList.begin(), planesList.end());
}

void
McCAD::Decomposition::FacesListGenerator::mergeCylindersList(Standard_Real& boxDiagonalLength){
    SurfacesMerger{}(cylindersList, boxDiagonalLength);
    facesList.insert(facesList.end(), cylindersList.begin(), cylindersList.end());
}

void
McCAD::Decomposition::FacesListGenerator::mergeToriList(Standard_Real& boxDiagonalLength){
    SurfacesMerger{}(toriList, boxDiagonalLength);
    facesList.insert(facesList.end(), toriList.begin(), toriList.end());
}
