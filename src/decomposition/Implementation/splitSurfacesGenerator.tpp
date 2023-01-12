/** ********************************************************************
* @brief    A template function that creates a plane through a line edge and an axis.
* @param    surface is either a gb_Cylinder or gb_Cone.
* @param    edge is a McCAD edge object.
* @return   an optional OCCT face.
* @date     11/11/2022
* @modified
* @author   Moataz Harb
* **********************************************************************/
template <typename gb_surfaceType>
std::optional<TopoDS_Face>
McCAD::Decomposition::SplitSurfaceGenerator::generatePlaneOnLineAxis(
        const gb_surfaceType& surface,
        const std::shared_ptr<Geometry::Edge>& edge){
    gp_Pnt edgeStart{edge->accessEImpl()->startPoint},
           edgeMid{edge->accessEImpl()->middlePoint},
           edgeEnd{edge->accessEImpl()->endPoint},
           surfaceOrigin{surface.Location()};
    gp_Vec firstVec(edgeStart, surfaceOrigin),
           secondVec(edgeEnd, surfaceOrigin);
    gp_Dir firstDir(firstVec), secondDir(secondVec);
    gp_Dir splitSurfNormal = firstDir.Crossed(secondDir);
    gp_Pln splitSurf(edgeMid, splitSurfNormal);
    return BRepBuilderAPI_MakeFace(splitSurf).Face();
}
