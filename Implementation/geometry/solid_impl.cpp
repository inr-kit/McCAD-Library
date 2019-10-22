// McCAD
#include "solid_impl.hpp"

McCAD::Geometry::Solid::Impl::Impl()
  : preproc{std::make_unique<Tools::Preprocessor>()},
    splitSolidList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedsubSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}{
}

McCAD::Geometry::Solid::Impl::~Impl(){
}

void
McCAD::Geometry::Solid::Impl::initiate(const TopoDS_Shape& aSolidShape){
    solidShape = aSolidShape;
    solid = TopoDS::Solid(solidShape);
}

void
McCAD::Geometry::Solid::Impl::repairSolid(){
    preproc->accessImpl()->removeSmallFaces(solidShape);
    solid = TopoDS::Solid(solidShape);
    preproc->accessImpl()->repairSolid(solid);
}

void
McCAD::Geometry::Solid::Impl::createOBB(Standard_Real bndBoxGap){
    // Calculate the bounding box of the solid.
    BRepBndLib::AddOBB(solid, obb);
    obb.Enlarge(bndBoxGap);
}

void
McCAD::Geometry::Solid::Impl::calcMeshDeflection(Standard_Real scalingFactor){
    // Calculate the oriented bounding box of the solid.
    meshDeflection = 2 * std::max(std::max(obb.XHSize(), obb.YHSize()),
                                  obb.ZHSize()) / scalingFactor;
    boxSquareLength = sqrt(obb.SquareExtent());
}

void
McCAD::Geometry::Solid::Impl::updateEdgesConvexity(const Standard_Real& angleTolerance){
    TopTools_IndexedDataMapOfShapeListOfShape mapEdgeFace;
    TopExp::MapShapesAndAncestors(solid, TopAbs_EDGE, TopAbs_FACE, mapEdgeFace);
    TopTools_ListOfShape facesList;
    for (Standard_Integer edgeNumber = 1; edgeNumber <= mapEdgeFace.Extent();
         ++edgeNumber){
        TopoDS_Edge edge = TopoDS::Edge(mapEdgeFace.FindKey(edgeNumber));
        BRepAdaptor_Curve curveAdaptor;
        curveAdaptor.Initialize(edge);
        facesList = mapEdgeFace.FindFromKey(edge);
        if(facesList.Extent() != Standard_Integer(2)){
            continue;
        }
        TopTools_ListIteratorOfListOfShape iterFace(facesList);
        TopoDS_Face firstFace = TopoDS::Face(iterFace.Value());
        iterFace.Next();
        TopoDS_Face secondFace = TopoDS::Face(iterFace.Value());

        Standard_Real start, end;
        Handle_Geom_Curve curve = BRep_Tool::Curve(edge, start, end);
        gp_Pnt startPoint;
        gp_Vec vector;
        curve->D1(start, startPoint, vector);
        gp_Dir direction(vector);

        // Get the normals of each surface
        gp_Dir firstNormal = Tools::normalOnFace(firstFace, startPoint);
        gp_Dir secondNormal = Tools::normalOnFace(secondFace, startPoint);
        Standard_Real angle = firstNormal.AngleWithRef(secondNormal, direction);

        if(std::abs(angle) < angleTolerance){
            angle = Standard_Real(0);
        }
        // The edge is convex.
        if( angle < Standard_Real(0) && edge.Orientation() == TopAbs_REVERSED){
            edge.Convex(1);
        } else if(angle > Standard_Real(0) && edge.Orientation() == TopAbs_FORWARD){
            edge.Convex(1);
        } else if (angle == Standard_Real(0)){
            // edge if flat
            edge.Convex(2);
        } else{
            // edge is concave
            edge.Convex(0);
        }
    }
}
/*
void
McCAD::Geometry::Solid::Impl::generateSurfacesList(){
    // Generate a list of faces of the solid.
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

void
McCAD::Geometry::Solid::Impl::mergeSurfaces(std::vector<std::unique_ptr<BoundSurface>>& surfacesList){
    if (surfacesList.size() < 2){
        return;
    }
    for (Standard_Integer i = 0; i <= surfacesList.size() - 2; ++i){
        for (Standard_Integer j = i+1; j <= surfacesList.size() - 1; ++j){
            //std::cout << i << " , " << j << " , " << surfacesList.size() << std::endl;
            if (*surfacesList[i] == *surfacesList[j]){
                //std::cout << "*** equal" << std::endl;
                surfacesList[j]->accessSImpl()->surfaceNumber =
                        surfacesList[i]->accessSImpl()->surfaceNumber;
                /*
                // Save surfaces to step file for comparison/debugging.
                STEPControl_Writer writer6;
                writer6.Transfer(surfacesList[j]->accessSImpl()->face,
                                 STEPControl_StepModelType::STEPControl_AsIs);
                writer6.Transfer(surfacesList[i]->accessSImpl()->face,
                                 STEPControl_StepModelType::STEPControl_AsIs);
                Standard_Integer kk = 0;
                std::string filename = "../examples/bbox/equalsurfaces";
                std::string suffix = ".stp";
                while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
                    ++kk;
                }
                filename += std::to_string(kk);
                filename += suffix;
                writer6.Write(filename.c_str());

                // Test if the two surfaces can be fused.
                if (*surfacesList[i] << *surfacesList[j]){
                    //std::cout << "*** equal fuse" << std::endl;
                    if (surfacesList[i]->getSurfaceType() == Tools::toTypeName(GeomAbs_Plane)){
                        TopoDS_Face newFace = Tools::PlaneFuser{}(
                                    surfacesList[i]->accessSImpl()->face,
                                    surfacesList[j]->accessSImpl()->face);
                        std::unique_ptr<BoundSurface> newboundSurface =
                                std::move(generateSurface(newFace));
                        newboundSurface->accessSImpl()->surfaceNumber =
                                surfacesList[i]->accessSImpl()->surfaceNumber;
                        // Add triangles of surface i.
                        for (Standard_Integer k = 0; k <=
                             surfacesList[i]->accessBSImpl()->meshTrianglesList.size() - 1; ++k){
                            newboundSurface->accessBSImpl()->meshTrianglesList.push_back(
                                        std::move(surfacesList[i]->accessBSImpl()->meshTrianglesList[k]));
                        }
                        // Add triangles of surface j.
                        for (Standard_Integer k = 0; k <=
                             surfacesList[j]->accessBSImpl()->meshTrianglesList.size() - 1; ++k){
                            newboundSurface->accessBSImpl()->meshTrianglesList.push_back(
                                        std::move(surfacesList[j]->accessBSImpl()->meshTrianglesList[k]));
                        }
                        // Combine edges.
                        newboundSurface->accessBSImpl()->combineEdges(
                                    surfacesList[i]->accessBSImpl()->edgesList);
                        newboundSurface->accessBSImpl()->combineEdges(
                                    surfacesList[j]->accessBSImpl()->edgesList);
                        // Erase pointer surfacesList[j] & [i] from surfacesList.
                        //std::cout << "equal, erase two: " << i << " , " << j << " , " << surfacesList.size() << std::endl;
                        surfacesList.erase(surfacesList.begin() + j);
                        --j;
                        surfacesList.erase(surfacesList.begin() + i);
                        --i;
                        surfacesList.push_back(std::move(newboundSurface));
                        break;
                    }
                } else{
                    //std::cout << "*** equal, erase one" << std::endl;
                    // Erase pointer surfacesList[j] from surfacesList.
                    /*
                    STEPControl_Writer writer7;
                    writer7.Transfer(surfacesList[j]->accessSImpl()->face,
                                     STEPControl_StepModelType::STEPControl_AsIs);
                    std::string filename = "../examples/bbox/equalsurfacedelete";
                    std::string suffix = ".stp";
                    while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
                        ++kk;
                    }
                    filename += std::to_string(kk);
                    filename += suffix;
                    writer7.Write(filename.c_str());

                    surfacesList.erase(surfacesList.begin() + j);
                    --j;
                }
                if (surfacesList.size() < 2){
                    return;
                }
            }
        }
    }
}
*/
