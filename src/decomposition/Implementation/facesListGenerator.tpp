//C++
#include <typeinfo>
#include <memory>
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
McCAD::Decomposition::FacesListGenerator::operator()(solidObjType& solidObj,
                                                     const Standard_Real& precision,
                                                     const Standard_Real& edgeTolerance,
                                                     const Standard_Real& faceTolerance){
    Tools::Preprocessor preproc{precision, faceTolerance};
    TopoDS_Face face;
    TopoDS_Solid solid = solidObj->accessSImpl()->solid;
    Standard_Integer faceNumber = 0;
    for(const auto& aFace : detail::ShapeView<TopAbs_FACE>{solid}){
        face = aFace;
        BRepTools::Update(face);
        if (!preproc.accessImpl()->checkFace(face)){
            ++faceNumber;
            preproc.accessImpl()->fixFace(face);
            std::shared_ptr<Geometry::BoundSurface> boundSurface =
                    SurfaceObjCreator{}(face,
                                        solidObj->accessSImpl()->boxDiagonalLength,
                                        edgeTolerance);
            boundSurface->accessSImpl()->surfaceNumber = faceNumber;
            if (boundSurface->accessBSImpl()->generateMesh(
                        solidObj->accessSImpl()->meshDeflection)){
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
    if (typeid(solidObjType) == typeid(std::shared_ptr<McCAD::Geometry::PLSolid>)){
        mergePlanesList(solidObj->accessSImpl()->boxDiagonalLength);
        solidObj->accessSImpl()->planesList = planesList;
    } else if (typeid(solidObjType) == typeid(std::shared_ptr<McCAD::Geometry::CYLSolid>)){
        mergePlanesList(solidObj->accessSImpl()->boxDiagonalLength);
        mergeCylindersList(solidObj->accessSImpl()->boxDiagonalLength);
        solidObj->accessSImpl()->planesList = planesList;
        solidObj->accessSImpl()->cylindersList = cylindersList;
    } else if (typeid(solidObjType) == typeid(std::shared_ptr<McCAD::Geometry::TORSolid>)){
        mergePlanesList(solidObj->accessSImpl()->boxDiagonalLength);
        mergeToriList(solidObj->accessSImpl()->boxDiagonalLength);
        solidObj->accessSImpl()->planesList = planesList;
        solidObj->accessSImpl()->toriList = toriList;
    } else return;
}

void
McCAD::Decomposition::FacesListGenerator::mergePlanesList(const Standard_Real& boxDiagonalLength){
    SurfacesMerger{}(planesList, boxDiagonalLength);
    facesList.insert(facesList.end(), planesList.begin(), planesList.end());
}

void
McCAD::Decomposition::FacesListGenerator::mergeCylindersList(const Standard_Real& boxDiagonalLength){
    SurfacesMerger{}(cylindersList, boxDiagonalLength);
    facesList.insert(facesList.end(), cylindersList.begin(), cylindersList.end());
}

void
McCAD::Decomposition::FacesListGenerator::mergeToriList(const Standard_Real& boxDiagonalLength){
    SurfacesMerger{}(toriList, boxDiagonalLength);
    facesList.insert(facesList.end(), toriList.begin(), toriList.end());
}
