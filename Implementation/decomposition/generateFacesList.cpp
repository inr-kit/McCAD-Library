//McCAD
#include "generateFacesList.hpp"
#include "surfaceObjCerator.hpp"
#include "surfacesMerger.hpp"
//OCC
#include <TopoDS_Solid.hxx>
#include <BRepTools.hxx>

McCAD::Decomposition::FacesListGenerator::FacesListGenerator(){
}

McCAD::Decomposition::FacesListGenerator::~FacesListGenerator(){
}

//General Solid
std::shared_ptr<McCAD::Geometry::BoundSurface>
McCAD::Decomposition::GenerateFacesList::operator()(const TopoDS_Face& face,
                                                    Standard_Real& boxDiagonalLength,
                                                    Standard_Integer mode){
    return SurfaceObjCreator{}(face, boxDiagonalLength, mode);
}

std::vector<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::GenerateFacesList::operator()(
        McCAD::Geometry::PLSolid& solidObj){
    // Generate a list of solid faces.
    TopoDS_Solid solid = solidObj.accessSImpl()->solid;
    TopoDS_Face face;
    Standard_Integer faceNumber = 0;
    std::vector<std::shared_ptr<Geometry::BoundSurface>> facesList;
    std::vector<std::shared_ptr<Geometry::BoundSurface>> planesList;
    for(const auto& aFace : detail::ShapeView<TopAbs_FACE>{solid}){
        // Update UV points of the face.
        face = aFace;
        BRepTools::Update(face);
        Standard_Boolean rejectCondition = preproc->accessImpl()->checkFace(face);
        if (!rejectCondition){
            ++faceNumber;
            preproc->accessImpl()->fixFace(face);
            std::shared_ptr<Geometry::BoundSurface> boundSurface =
                    SurfaceObjCreator{}(face,
                                        solidObj.accessSImpl()->boxDiagonalLength);
            boundSurface->accessSImpl()->surfaceNumber = faceNumber;
            if (boundSurface->accessBSImpl()->generateMesh(
                        solidObj.accessSImpl()->meshDeflection)){
                boundSurface->accessBSImpl()->generateEdges();
                planesList.push_back(std::move(boundSurface));
            }
        }
    }
    //std::cout << "     - There are " << planesList.size() << " planes in the "
    //                                                         "solid" << std::endl;
    solidObj.accessPSImpl()->planesList = planesList;
    SurfacesMerger{}(planesList, solidObj.accessSImpl()->boxDiagonalLength);
    facesList.insert(facesList.end(), planesList.begin(), planesList.end());
    return facesList;
}

std::vector<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::GenerateFacesList::operator()(
        McCAD::Geometry::CYLSolid& solidObj){
    TopoDS_Solid solid = solidObj.accessSImpl()->solid;
    //std::cout << "solidObj.accessSImpl()->solid" << std::endl;
    TopoDS_Face face;
    Standard_Integer faceNumber = 0;
    std::vector<std::shared_ptr<Geometry::BoundSurface>> facesList;
    std::vector<std::shared_ptr<Geometry::BoundSurface>> planesList;
    std::vector<std::shared_ptr<Geometry::BoundSurface>> cylindersList;
    for(const auto& aFace : detail::ShapeView<TopAbs_FACE>{solid}){
        face = aFace;
        //std::cout << "face = aFace;" << std::endl;
        BRepTools::Update(face);
        //std::cout << "BRepTools::Update(face);" << std::endl;
        Standard_Boolean rejectCondition = preproc->accessImpl()->checkFace(face);
        //std::cout << "checkFace(face)" << std::endl;
        if (!rejectCondition){
            ++faceNumber;
            preproc->accessImpl()->fixFace(face);
            //std::cout << "fixFace(face)" << std::endl;
            std::shared_ptr<Geometry::BoundSurface> boundSurface =
                    SurfaceObjCreator{}(face,
                                        solidObj.accessSImpl()->boxDiagonalLength);
            boundSurface->accessSImpl()->surfaceNumber = faceNumber;
            if (boundSurface->accessBSImpl()->generateMesh(
                        solidObj.accessSImpl()->meshDeflection)){
                boundSurface->accessBSImpl()->generateEdges();
                if(boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Plane)){
                    planesList.push_back(std::move(boundSurface));
                } else if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Cylinder)){
                    cylindersList.push_back(std::move(boundSurface));
                }
            } else{
            //std::cout << "face rejected" << std::endl;
            continue;
            }
        }
    }
    //std::cout << "     - There are " << planesList.size() <<
    //             " planes in the solid" << std::endl;
    solidObj.accessCSImpl()->planesList = planesList;
    SurfacesMerger{}(planesList, solidObj.accessSImpl()->boxDiagonalLength);
    facesList.insert(facesList.end(), planesList.begin(), planesList.end());
    //std::cout << "     - There are " << cylindersList.size() <<
    //             " cylinders in the solid" << std::endl;
    solidObj.accessCSImpl()->cylindersList = cylindersList;
    SurfacesMerger{}(cylindersList, solidObj.accessSImpl()->boxDiagonalLength);
    facesList.insert(facesList.end(), cylindersList.begin(), cylindersList.end());
    return facesList;
}

std::vector<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::GenerateFacesList::operator()(
        McCAD::Geometry::TORSolid& solidObj){
    std::cout << "generate torus surface" << std::endl;
}
