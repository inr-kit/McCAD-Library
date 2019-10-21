//McCAD
#include "generateFacesList.hpp"
#include "planarSolid_impl.hpp"

template<typename solidObjType>
std::vector<std::shared_ptr<BoundSurface>>
McCAD::Decomposition::GenerateFacesListoperator::operator()(
        const McCAD::Geometry::PLSolid::Impl& solidObj){
    // Generate a list of faces of the solid.
    TopoDS_Solid solid = solidObj.solid;
    TopoDS_Face face;
    Standard_Integer faceNumber = 0;
    std::vector<std::unique_ptr<BoundSurface>> planesList;
    for(const auto& aFace : ShapeView<TopAbs_FACE>{solid}){
        // Update UV points of the face.
        face = aFace;
        BRepTools::Update(face);
        Standard_Boolean rejectCondition = preproc->accessImpl()->checkFace(face);
        if (!rejectCondition){
            ++faceNumber;
            //std::cout << "faceNumber: " << faceNumber << std::endl;
            preproc->accessImpl()->fixFace(face);
            //std::cout << "face fixed: " << std::endl;
            std::unique_ptr<BoundSurface> boundSurface = std::move(generateSurface(face));
            //boundSurface->accessSImpl()->initiate(face);
            boundSurface->accessSImpl()->surfaceNumber = faceNumber;
            if (boundSurface->accessBSImpl()->generateMesh(meshDeflection)){
                boundSurface->accessBSImpl()->generateEdges();
                //std::cout << "length of egdes list: " <<
                //             boundSurface->accessBSImpl()->edgesList.size() <<
                //             std::endl;
                if(boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Plane)){
                    planesList.push_back(std::move(boundSurface));
                }
                // The other two types in McCADDecompSolid are to be added later.
            }
        } else{
            //std::cout << "face rejected" << std::endl;
            continue;
        }
    }
    std::cout << "     - There are " << planesList.size() <<
                 " planes in the solid" << std::endl;
    mergeSurfaces(planesList);
    //std::cout << "merged planes list: " << planesList.size() << std::endl;
    for (Standard_Integer i = 0; i <= planesList.size() - 1; ++i){
        facesList.push_back(std::move(planesList[i]));
    }
    //std::cout << "merged faces list: " << facesList.size() << std::endl;
}

std::unique_ptr<McCAD::Geometry::BoundSurface>
McCAD::Geometry::Solid::Impl::generateSurface(const TopoDS_Face& face,
                                              Standard_Integer mode){
    if (mode == Standard_Integer(0)){
        //std::cout << "mode 0 " << std::endl;
        BRepAdaptor_Surface surface(face, Standard_True);
        //std::cout << "BRepAdaptor_Surface" << std::endl;
        GeomAdaptor_Surface AdaptorSurface = surface.Surface();
        //std::cout << "GeomAdaptor_Surface" << std::endl;
        if (AdaptorSurface.GetType() == GeomAbs_Plane){
            //std::cout << preproc->accessImpl()->getSurfTypeName(
            //                 AdaptorSurface.GetType()) << std::endl;
            std::unique_ptr<BoundSurfacePlane> boundSurfacePlane =
                    std::make_unique<BoundSurfacePlane>();
            boundSurfacePlane->setSurfaceType(Tools::toTypeName(GeomAbs_Plane));
            boundSurfacePlane->accessSImpl()->initiate(face);
            boundSurfacePlane->accessBSPImpl()->generateExtendedPlane(boxSquareLength);
            return boundSurfacePlane;
        } else if (AdaptorSurface.GetType() == GeomAbs_Cylinder){
            return nullptr;
        } else{
            return nullptr;
        }
    }
    return nullptr;
}
