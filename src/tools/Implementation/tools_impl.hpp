#ifndef TOOLSIMPL_HPP
#define TOOLSIMPL_HPP

// C++
#include <algorithm>
#include <vector>
// McCAD
#include "tools.hpp"
// OCC
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>

namespace McCAD::Tools{
  class Preprocessor::Impl{
  public:
    Impl() = default;

    void removeSmallFaces(TopoDS_Shape& solidShape,
                          Standard_Real precision = 1.0e-3,
                          Standard_Real maxTolerance = 1.0e-3);
    void repairSolid(TopoDS_Solid& solid);
    Standard_Boolean checkFace(const TopoDS_Face& face,
                               Standard_Real tolerance = 1.0e-3);
    void fixFace(TopoDS_Face& face,
                 Standard_Real precision = 1.0e-7,
                 Standard_Real maxTolerance = 1.0e-3);
  };
}

#endif //TOOLSIMPL_HPP
