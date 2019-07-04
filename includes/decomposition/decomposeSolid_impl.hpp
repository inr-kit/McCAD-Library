#ifndef DECOMPOSESOLID_IMPL_HPP
#define DECOMPOSESOLID_IMPL_HPP

// C++
#include <vector>
#include <assert.h>
// McCAD
#include "decomposeSolid.hpp"
//#include "boundSurface.hpp"
#include "boundSurfacePlane.hpp"
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

namespace McCAD::Decomposition{
  class DecomposeSolid::Impl {
  public:
    Impl() = default;

    TopoDS_Solid solid;
    Standard_Real meshDeflection;
    Standard_Real boxSquareLength;
    Standard_Integer recurrenceDepth = 0;
    Standard_Boolean splitSurface = Standard_False;
    std::vector<McCAD::Decomposition::BoundSurface*> facesList;
    
    void initiate(const TopoDS_Solid& aSolid);
    void perform(Standard_Integer recurrenceDepth);
    void generateSurfacesList();
    McCAD::Decomposition::BoundSurface* generateSurface(const TopoDS_Face& face, Standard_Integer mode = 0);
    void generateEdges(McCAD::Decomposition::BoundSurface* boundSurface);
    void mergeSurfaces(std::vector<McCAD::Decomposition::BoundSurface*> planesList);
    void judgeDecomposeSurfaces();

  private:

  };
}

#endif //DECOMPOSESOLID_IMPL_HPP
