//McCAD
#include "generateFacesList.hpp"
#include "planarSolid_impl.hpp"
#include "tools_impl.hpp"

// Planar solids
std::vector<std::shared_ptr<Geometry::BoundSurface>>
McCAD::Decomposition::GenerateFacesList::operator()(
        const McCAD::Geometry::PLSolid& solidObj){
    // Generate a list of solid faces.
    TopoDS_Solid solid = solidObj.accesSImpl()->solid;
    Standard_Integer faceNumber = 0;
    std::vector<std::shared_ptr<Geometry::BoundSurface>> facesList;
    std::vector<std::shared_ptr<Geometry::BoundSurface>> planesList;
    for(const auto& face : ShapeView<TopAbs_FACE>{solid}){
        // Update UV points of the face.
        BRepTools::Update(face);
        Standard_Boolean rejectCondition = preproc->accessImpl()->checkFace(face);
        if (!rejectCondition){
            ++faceNumber;
            preproc->accessImpl()->fixFace(face);
            std::shared_ptr<Geometry::BoundSurface> boundSurface =
                    generateSurface<McCAD::Geometry::PLSolid>(
                        face, solidObj.accesSImpl()->boxSquareLength);
            boundSurface->accessSImpl()->surfaceNumber = faceNumber;
            if (boundSurface->accessBSImpl()->generateMesh(meshDeflection)){
                boundSurface->accessBSImpl()->generateEdges();
                planesList.push_back(std::move(boundSurface));
            }
        }
    }
    std::cout << "     - There are " << planesList.size() << " planes in the "
                                                             "solid" << std::endl;
    mergeSurfaces(planesList);
    facesList.insert(facesList.end(), planesList.begin(), planesList.end());
    return facesList;
}

template<>
std::shared_ptr<McCAD::Geometry::BoundSurface>
McCAD::Decomposition::GenerateFacesList::generateSurface<McCAD::Geometry::PLSolid>(
        const TopoDS_Face& face, Standard_Integer mode, Standard_Real& boxSquareLength){
    if (mode == Standard_Integer(0)){
        std::shared_ptr<Geometry::BoundSurfacePlane> boundSurfacePlane =
                std::make_shared<Geometry::BoundSurfacePlane>();
        boundSurfacePlane->setSurfaceType(Tools::toTypeName(GeomAbs_Plane));
        boundSurfacePlane->accessSImpl()->initiate(face);
        boundSurfacePlane->accessBSPImpl()->generateExtendedPlane(boxSquareLength);
        return boundSurfacePlane;
    } else return nullptr;
}

// Cylindrical solids.
std::vector<std::shared_ptr<Geometry::BoundSurface>>
McCAD::Decomposition::GenerateFacesList::operator()(
        const McCAD::Geometry::CYLSolid& solidObj){
    TopoDS_Solid solid = solidObj.accesSImpl()->solid;
    Standard_Integer faceNumber = 0;
    std::vector<std::shared_ptr<Geometry::BoundSurface>> facesList;
    std::vector<std::shared_ptr<Geometry::BoundSurface>> planesList;
    std::vector<std::shared_ptr<Geometry::BoundSurface>> cylindersList;
    for(const auto& face : ShapeView<TopAbs_FACE>{solid}){
        BRepTools::Update(face);
        Standard_Boolean rejectCondition = preproc->accessImpl()->checkFace(face);
        if (!rejectCondition){
            ++faceNumber;
            preproc->accessImpl()->fixFace(face);
            std::shared_ptr<Geometry::BoundSurface> boundSurface =
                    generateSurface<McCAD::Geometry::CYLSolid>(face);
            boundSurface->accessSImpl()->surfaceNumber = faceNumber;
            if (boundSurface->accessBSImpl()->generateMesh(meshDeflection)){
                boundSurface->accessBSImpl()->generateEdges();
                if(boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Plane)){
                    planesList.push_back(std::move(boundSurface));
                }
                } else if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Cylinder)){
                    cylindersList.push_back(std::move(boundSurface));
                }
            } else{
            //std::cout << "face rejected" << std::endl;
            continue;
        }
    }
    std::cout << "     - There are " << planesList.size() <<
                 " planes in the solid" << std::endl;
    mergeSurfaces(planesList);
    facesList.insert(facesList.end(), planesList.begin(), planesList.end());
    std::cout << "     - There are " << cylindersList.size() <<
                 " cylinders in the solid" << std::endl;
    mergeSurfaces(cylindersList);
    facesList.insert(facesList.end(), cylindersList.begin(), cylindersList.end());
    return facesList;
}

template<>
std::shared_ptr<McCAD::Geometry::BoundSurface>
McCAD::Decomposition::GenerateFacesList::generateSurface<McCAD::Geometry::CYLSolid>(
        const TopoDS_Face& face, Standard_Integer mode, Standard_Real& boxSquareLength){
    if (mode == Standard_Integer(0)){
        BRepAdaptor_Surface surface(face, Standard_True);
        GeomAdaptor_Surface AdaptorSurface = surface.Surface();
        if (AdaptorSurface.GetType() == GeomAbs_Plane){
            std::shared_ptr<Geometry::BoundSurfacePlane> boundSurfacePlane =
                    std::make_shared<Geometry::BoundSurfacePlane>();
            boundSurfacePlane->setSurfaceType(Tools::toTypeName(GeomAbs_Plane));
            boundSurfacePlane->accessSImpl()->initiate(face);
            boundSurfacePlane->accessBSPImpl()->generateExtendedPlane(boxSquareLength);
            return boundSurfacePlane;
        } else if (AdaptorSurface.GetType() == GeomAbs_Cylinder){
            std::shared_ptr<Geometry::BoundSurfaceCyl> boundSurfaceCyl =
                    std::make_shared<Geometry::BoundSurfaceCyl>();
            boundSurfaceCyl->setSurfaceType(Tools::toTypeName(GeomAbs_Cylinder));
            boundSurfaceCyl->accessSImpl()->initiate(face);
            //boundSurfaceCyl->accessBSCImpl()->generateExtendedPlane(boxSquareLength);
            return boundSurfaceCyl;
        }
    }
    return nullptr;
}
