#ifndef DECOMPOSESOLID_IMPL_HPP
#define DECOMPOSESOLID_IMPL_HPP

// C++
#include <vector>
#include <assert.h>
// McCAD
#include "decomposeSolid.hpp"
#include "tools_impl.hpp"
#include "surface_impl.hpp"
#include "boundSurface_impl.hpp"
#include "boundSurfacePlane_impl.hpp"
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

    McCAD::Tools::Preprocessor preproc;
    TopoDS_Solid solid;
    std::vector<std::unique_ptr<McCAD::Decomposition::BoundSurface>> facesList;
    Standard_Real meshDeflection;
    Standard_Real boxSquareLength;
    Standard_Integer recurrenceDepth = 0;
    Standard_Boolean splitSurface = Standard_False;

    void initiate(const TopoDS_Solid& solid);
    void perform();
    void generateSurfacesList();
    std::unique_ptr<McCAD::Decomposition::BoundSurface> generateSurface(const TopoDS_Face& face, Standard_Integer mode = 0);
    void generateEdges(const std::unique_ptr<McCAD::Decomposition::BoundSurface>& surface);
    void mergeSurfaces();
    void judgeDecomposeSurfaces();
    std::string getSurfTypeName(const Standard_Integer& index);

  private:

  };
}

#endif //DECOMPOSESOLID_IMPL_HPP
