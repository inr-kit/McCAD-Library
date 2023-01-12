#ifndef TOOLSIMPL_HPP
#define TOOLSIMPL_HPP

// McCAD
#include "tools.hpp"
// OCCT
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>

namespace McCAD::Tools{
  class Preprocessor::Impl{
  public:
      Impl();
      Impl(const double& precision, const double& faceTolerance);
      ~Impl();

      double precision{1.0e-6}, maxTolerance{1.0e-3};
      void removeSmallFaces(TopoDS_Shape& solidShape);
      void repairSolid(TopoDS_Solid& solid);
      bool checkFace(const TopoDS_Face& face);
      void fixFace(TopoDS_Face& face);
  };
}

#endif //TOOLSIMPL_HPP
