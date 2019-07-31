#ifndef SPLITSOLID_IMPL_HPP
#define SPLITSOLID_IMPL_HPP

// C++
#include <vector>
// McCAD
#include "splitsolid.hpp"
// OCC
#include <Handle_TopTools_HSequenceOfShape.hxx>
#include <BRepBndLib.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>

namespace McCAD::Decomposition{
  class SplitSolid::Impl {
  public:
    Impl() = default;

    Bnd_Box bndBox;
    Standard_Real boxSquareLength;
    TopoDS_Shape boundingBox

  private:

  };
}

#endif //SPLITSOLID_IMPL_HPP
