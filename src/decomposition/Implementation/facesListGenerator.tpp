//C++
#include <typeinfo>
#include <memory>
//McCAD
#include "facesListGenerator.hpp"
#include "planarSolid_impl.hpp"
#include "cylSolid_impl.hpp"
#include "torSolid_impl.hpp"
#include "conSolid_impl.hpp"
#include "ShapeView.hpp"
#include "tools_impl.hpp"
#include "surfaceObjCerator.hpp"
#include "surfacesMerger.hpp"
#include "SurfaceUtilities.hpp"
//OCC
#include <TopoDS_Face.hxx>
#include <TopoDS_Solid.hxx>
#include <BRepTools.hxx>

/** ********************************************************************
* @brief    An operator that creates a list of bound surface objects.
* @param    solidObj is a McCAD solid object.
* @param    precision is used in comparing numerical values and is set on the inputConfig file.
* @param    edgeTolerance is used in comparing OCCT edges and is set on the inputConfig file.
* @param    faceTolerance is used in comparing OCCT faces and is set on the inputConfig file.
* @param    angularTolerance is used in comparing angles and is set on the inputConfig file.
* @param    distanceTolerance is used in comparing distances and is set on the inputConfig file.
* @param    parameterTolerance is used in comparing UV parameters and is set on the inputConfig file.
* @return   A vector of McCAD boundary surface objects.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
template <typename solidObjType>
std::vector<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::FacesListGenerator::operator()(solidObjType& solidObj,
                                                     const double& precision,
                                                     const double& edgeTolerance,
                                                     const double& faceTolerance,
                                                     const double& angularTolerance,
                                                     const double& distanceTolerance,
                                                     const double& parameterTolerance){
    Tools::Preprocessor preproc{precision, faceTolerance};
    TopoDS_Face face;
    TopoDS_Solid solid = solidObj->accessSImpl()->solid;
    int faceNumber = 0;
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
                boundSurface->accessBSImpl()->generateEdges(parameterTolerance);
                if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Plane)){
                    planesList.push_back(std::move(boundSurface));
                } else if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Cylinder)){
                    cylindersList.push_back(std::move(boundSurface));
                } else if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Torus)){
                    toriList.push_back(std::move(boundSurface));
                } else if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Cone)){
                    conesList.push_back(std::move(boundSurface));
                }
            } else continue;
        }
    }
    addListsToSolidObj(solidObj, precision, edgeTolerance, angularTolerance, distanceTolerance);
    return facesList;
}

/** ********************************************************************
* @brief    An function that adds the created McCAD surface objects to the parent McCAD solid object.
* @param    solidObj is a McCAD solid object.
* @param    precision is used in comparing numerical values and is set on the inputConfig file.
* @param    edgeTolerance is used in comparing OCCT edges and is set on the inputConfig file.
* @param    angularTolerance is used in comparing angles and is set on the inputConfig file.
* @param    distanceTolerance is used in comparing distances and is set on the inputConfig file.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
template <typename solidObjType>
void
McCAD::Decomposition::FacesListGenerator::addListsToSolidObj(solidObjType& solidObj,
                                                             const double& precision,
                                                             const double& edgeTolerance,
                                                             const double& angularTolerance,
                                                             const double& distanceTolerance){
    if (typeid(solidObjType) == typeid(std::shared_ptr<McCAD::Geometry::PLSolid>)){
        mergePlanesList(solidObj->accessSImpl()->boxDiagonalLength, precision,
                        edgeTolerance, angularTolerance, distanceTolerance);
        solidObj->accessSImpl()->planesList = planesList;
    } else if (typeid(solidObjType) == typeid(std::shared_ptr<McCAD::Geometry::CYLSolid>)){
        mergePlanesList(solidObj->accessSImpl()->boxDiagonalLength, precision,
                        edgeTolerance, angularTolerance, distanceTolerance);
        mergeCylindersList(solidObj->accessSImpl()->boxDiagonalLength, precision,
                           edgeTolerance, angularTolerance, distanceTolerance);
        solidObj->accessSImpl()->planesList = planesList;
        solidObj->accessSImpl()->cylindersList = cylindersList;
    } else if (typeid(solidObjType) == typeid(std::shared_ptr<McCAD::Geometry::TORSolid>)){
        mergePlanesList(solidObj->accessSImpl()->boxDiagonalLength, precision,
                        edgeTolerance, angularTolerance, distanceTolerance);
        mergeToriList(solidObj->accessSImpl()->boxDiagonalLength, precision,
                      edgeTolerance, angularTolerance, distanceTolerance);
        solidObj->accessSImpl()->planesList = planesList;
        solidObj->accessSImpl()->toriList = toriList;
    } else if (typeid(solidObjType) == typeid(std::shared_ptr<McCAD::Geometry::MXDSolid>)){
        mergePlanesList(solidObj->accessSImpl()->boxDiagonalLength, precision,
                        edgeTolerance, angularTolerance, distanceTolerance);
        mergeCylindersList(solidObj->accessSImpl()->boxDiagonalLength, precision,
                           edgeTolerance, angularTolerance, distanceTolerance);
        mergeToriList(solidObj->accessSImpl()->boxDiagonalLength, precision,
                      edgeTolerance, angularTolerance, distanceTolerance);
        mergeConesList(solidObj->accessSImpl()->boxDiagonalLength, precision,
                           edgeTolerance, angularTolerance, distanceTolerance);
        solidObj->accessSImpl()->planesList = planesList;
        solidObj->accessSImpl()->cylindersList = cylindersList;
        solidObj->accessSImpl()->toriList = toriList;
        solidObj->accessSImpl()->conesList = conesList;
    } else if (typeid(solidObjType) == typeid(std::shared_ptr<McCAD::Geometry::CONSolid>)){
        mergePlanesList(solidObj->accessSImpl()->boxDiagonalLength, precision,
                        edgeTolerance, angularTolerance, distanceTolerance);
        mergeConesList(solidObj->accessSImpl()->boxDiagonalLength, precision,
                           edgeTolerance, angularTolerance, distanceTolerance);
        solidObj->accessSImpl()->planesList = planesList;
        solidObj->accessSImpl()->conesList = conesList;
    } else return;
}

void
McCAD::Decomposition::FacesListGenerator::mergePlanesList(const double& boxDiagonalLength,
                                                          const double& precision,
                                                          const double& edgeTolerance,
                                                          const double& angularTolerance,
                                                          const double& distanceTolerance){
    SurfacesMerger{}(planesList, boxDiagonalLength, precision, edgeTolerance,
                     angularTolerance, distanceTolerance);
    facesList.insert(facesList.end(), planesList.begin(), planesList.end());
}

void
McCAD::Decomposition::FacesListGenerator::mergeCylindersList(const double& boxDiagonalLength,
                                                             const double& precision,
                                                             const double& edgeTolerance,
                                                             const double& angularTolerance,
                                                             const double& distanceTolerance){
    SurfacesMerger{}(cylindersList, boxDiagonalLength, precision, edgeTolerance,
                     angularTolerance, distanceTolerance);
    facesList.insert(facesList.end(), cylindersList.begin(), cylindersList.end());
}

void
McCAD::Decomposition::FacesListGenerator::mergeToriList(const double& boxDiagonalLength,
                                                        const double& precision,
                                                        const double& edgeTolerance,
                                                        const double& angularTolerance,
                                                        const double& distanceTolerance){
    SurfacesMerger{}(toriList, boxDiagonalLength, precision, edgeTolerance,
                     angularTolerance, distanceTolerance);
    facesList.insert(facesList.end(), toriList.begin(), toriList.end());
}

void
McCAD::Decomposition::FacesListGenerator::mergeConesList(const double& boxDiagonalLength,
                                                         const double& precision,
                                                         const double& edgeTolerance,
                                                         const double& angularTolerance,
                                                         const double& distanceTolerance){
    SurfacesMerger{}(conesList, boxDiagonalLength, precision, edgeTolerance,
                     angularTolerance, distanceTolerance);
    facesList.insert(facesList.end(), conesList.begin(), conesList.end());
}
