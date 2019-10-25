//McCAD
#include "generateFacesList.hpp"
#include "tools_impl.hpp"
//OCC
#include <TopoDS_Solid.hxx>
#include <BRepTools.hxx>

// Planar solids
template<>
std::shared_ptr<McCAD::Geometry::BoundSurface>
McCAD::Decomposition::FacesListGenerator::generateSurface<McCAD::Geometry::PLSolid>(
        const TopoDS_Face& face, Standard_Real& boxSquareLength,
        Standard_Integer mode){
    //std::cout << "generateSurface<McCAD::Geometry::PLSolid>" << std::endl;
    if (mode == Standard_Integer(0)){
        std::shared_ptr<Geometry::BoundSurfacePlane> boundSurfacePlane =
                std::make_shared<Geometry::BoundSurfacePlane>();
        boundSurfacePlane->setSurfaceType(Tools::toTypeName(GeomAbs_Plane));
        boundSurfacePlane->accessSImpl()->initiate(face);
        boundSurfacePlane->accessBSPImpl()->generateExtendedPlane(boxSquareLength);
        return boundSurfacePlane;
    } else{
        //std::cout << "nullptr" << std::endl;
        return nullptr;
    }
}

std::vector<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::FacesListGenerator::operator()(
        const McCAD::Geometry::PLSolid& solidObj){
    //std::cout << "GenerateFacesList::operator()<McCAD::Geometry::PLSolid>" << std::endl;
    // Generate a list of solid faces.
    TopoDS_Solid solid = solidObj.accessSImpl()->solid;
    TopoDS_Face face;
    Standard_Integer faceNumber = 0;
    std::vector<BS> facesList;
    std::vector<BS> planesList;
    for(const auto& aFace : ShapeView<TopAbs_FACE>{solid}){
        // Update UV points of the face.
        face = aFace;
        BRepTools::Update(face);
        Standard_Boolean rejectCondition =
                Tools::Preprocessor{}.accessImpl()->checkFace(face);
        if (!rejectCondition){
            ++faceNumber;
            Tools::Preprocessor{}.accessImpl()->fixFace(face);
            BS boundSurface = generateSurface<McCAD::Geometry::PLSolid>(
                        face, solidObj.accessSImpl()->boxSquareLength);
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
    mergeSurfaces(planesList, solidObj.accessSImpl()->boxSquareLength);
    facesList.insert(facesList.end(), planesList.begin(), planesList.end());
    return facesList;
}

// Cylindrical solids.
template<>
std::shared_ptr<McCAD::Geometry::BoundSurface>
McCAD::Decomposition::FacesListGenerator::generateSurface<McCAD::Geometry::CYLSolid>(
        const TopoDS_Face& face, Standard_Real& boxSquareLength,
        Standard_Integer mode){
    if (mode == Standard_Integer(0)){
        //std::cout << "generateSurface" << std::endl;
        BRepAdaptor_Surface surface(face, Standard_True);
        GeomAdaptor_Surface AdaptorSurface = surface.Surface();
        if (AdaptorSurface.GetType() == GeomAbs_Plane){
            //std::cout << "<McCAD::Geometry::PLSolid>" << std::endl;
            std::shared_ptr<Geometry::BoundSurfacePlane> boundSurfacePlane =
                    std::make_shared<Geometry::BoundSurfacePlane>();
            boundSurfacePlane->setSurfaceType(Tools::toTypeName(GeomAbs_Plane));
            boundSurfacePlane->accessSImpl()->initiate(face);
            boundSurfacePlane->accessBSPImpl()->generateExtendedPlane(boxSquareLength);
            return boundSurfacePlane;
        } else if (AdaptorSurface.GetType() == GeomAbs_Cylinder){
            //std::cout << "<McCAD::Geometry::CYLSolid>" << std::endl;
            std::shared_ptr<Geometry::BoundSurfaceCyl> boundSurfaceCyl =
                    std::make_shared<Geometry::BoundSurfaceCyl>();
            boundSurfaceCyl->setSurfaceType(Tools::toTypeName(GeomAbs_Cylinder));
            boundSurfaceCyl->accessSImpl()->initiate(face);
            //boundSurfaceCyl->accessBSCImpl()->generateExtendedPlane(boxSquareLength);
            return boundSurfaceCyl;
        }
    }
    //std::cout << "nullptr" << std::endl;
    return nullptr;
}

std::vector<std::shared_ptr<McCAD::Geometry::BoundSurface>>
McCAD::Decomposition::FacesListGenerator::operator()(
        const McCAD::Geometry::CYLSolid& solidObj){
    //std::cout << "GenerateFacesList::operator()<McCAD::Geometry::CYLSolid>" << std::endl;
    TopoDS_Solid solid = solidObj.accessSImpl()->solid;
    //std::cout << "solidObj.accessSImpl()->solid" << std::endl;
    TopoDS_Face face;
    Standard_Integer faceNumber = 0;
    std::vector<BS> facesList;
    std::vector<BS> planesList;
    std::vector<BS> cylindersList;
    for(const auto& aFace : ShapeView<TopAbs_FACE>{solid}){
        face = aFace;
        //std::cout << "face = aFace;" << std::endl;
        BRepTools::Update(face);
        //std::cout << "BRepTools::Update(face);" << std::endl;
        Standard_Boolean rejectCondition = Tools::Preprocessor{}.accessImpl()->checkFace(face);
        //std::cout << "checkFace(face)" << std::endl;
        if (!rejectCondition){
            ++faceNumber;
            Tools::Preprocessor{}.accessImpl()->fixFace(face);
            //std::cout << "fixFace(face)" << std::endl;
            BS boundSurface = generateSurface<McCAD::Geometry::CYLSolid>(
                        face, solidObj.accessSImpl()->boxSquareLength);
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
            }
        }
    }
    //std::cout << "     - There are " << planesList.size() <<
    //             " planes in the solid" << std::endl;
    mergeSurfaces(planesList, solidObj.accessSImpl()->boxSquareLength);
    facesList.insert(facesList.end(), planesList.begin(), planesList.end());
    //std::cout << "     - There are " << cylindersList.size() <<
    //             " cylinders in the solid" << std::endl;
    mergeSurfaces(cylindersList, solidObj.accessSImpl()->boxSquareLength);
    facesList.insert(facesList.end(), cylindersList.begin(), cylindersList.end());
    return facesList;
}
