#ifndef DECOMPOSESOLID_IMPL_HPP
#define DECOMPOSESOLID_IMPL_HPP

// C++
#include <vector>
#include <assert.h>
// McCAD
#include "decomposeSolid.hpp"
#include "tools_impl.hpp"
#include "boundSurfacePlane_impl.hpp"
#include "boundSurface_impl.hpp"
#include "surface_impl.hpp"
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
    Impl(McCAD::Decomposition::DecomposeSolid* dcompSolid);
    ~Impl();

    McCAD::Decomposition::DecomposeSolid* decompSolid;
    TopoDS_Solid solid;
    std::vector<McCAD::Decomposition::BoundSurface*> facesList;
    
    void initiate(const TopoDS_Solid& aSolid);
    void perform();
    void generateSurfacesList();
    McCAD::Decomposition::BoundSurface* generateSurface(const TopoDS_Face& face, Standard_Integer mode = 0);
    void generateEdges(McCAD::Decomposition::BoundSurface* boundSurface);
    void mergeSurfaces(std::vector<McCAD::Decomposition::BoundSurface*> planesList);
    void judgeDecomposeSurfaces();

  private:

  };
}

#endif //DECOMPOSESOLID_IMPL_HPP
