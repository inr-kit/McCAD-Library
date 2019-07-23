#ifndef DECOMPOSESOLID_IMPL_HPP
#define DECOMPOSESOLID_IMPL_HPP

// C++
#include <vector>
#include <assert.h>
// McCAD
#include "decomposeSolid.hpp"
#include "tools_impl.hpp"
#include "edge_impl.hpp"
#include "surface_impl.hpp"
#include "boundSurface_impl.hpp"
#include "boundSurfacePlane_impl.hpp"
#include <Standard.hxx>
// OCC
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <Handle_Geom_Curve.hxx>

namespace McCAD::Decomposition{
  class DecomposeSolid::Impl {
  public:
    Impl() = default;

    McCAD::Tools::Preprocessor preproc;
    TopoDS_Solid solid;
    std::vector<std::unique_ptr<McCAD::Decomposition::BoundSurface>> facesList;
    Standard_Real meshDeflection;
    Standard_Real boxSquareLength;
    Standard_Integer recurrenceDepth = 0;
    Standard_Boolean splitSurface = Standard_False;

    void initiate(const TopoDS_Solid& solid);
    void perform();
    void updateEdgesConvexity(const Standard_Real& angleTolerance = 1.0e-4);
    void generateSurfacesList();
    std::unique_ptr<McCAD::Decomposition::BoundSurface> generateSurface(const TopoDS_Face& face, Standard_Integer mode = 0);
    void generateEdges(std::unique_ptr<McCAD::Decomposition::BoundSurface>& surface, Standard_Real uvTolerance = 1.0e-3);
    void mergeSurfaces(std::vector<std::unique_ptr<McCAD::Decomposition::BoundSurface>>& planesList);
    void judgeDecomposeSurfaces();

  private:

  };
}

#endif //DECOMPOSESOLID_IMPL_HPP
