#ifndef DECOMPOSESOLID_IMPL_HPP
#define DECOMPOSESOLID_IMPL_HPP

// C++
#include <vector>
// McCAD
#include "decomposeSolid.hpp"
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

namespace McCAD::Decomposition{
  class DecomposeSolid::Impl {
  public:
    Impl() = default;

    TopoDS_Solid solid;
    Standard_Real meshDeflection;
    Standard_Real boxSquareLength;
    Standard_Integer recurrenceLevel = 0;
    Standard_Boolean splitSurface = Standard_False;
    std::vector<McCAD::Decomposition::boundSurface*> facesList;

    void initiate(const TopoDS_Solid& aSolid);
    void perform(Standard_Integer recurrenceLevel);
    void generateSurfacesList(const TopoDS_Face& face, Standard_Integer mode = 0);
    void judgeDecomposeSurfaces();

  private:

  };
}

#endif //DECOMPOSESOLID_IMPL_HPP
