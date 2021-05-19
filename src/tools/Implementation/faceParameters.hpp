#ifndef FACEPARAMETERS_HPP
#define FACEPARAMETERS_HPP

// C++
#include <array>
#include <tuple>
#include <optional>
// McCAD
#include "SurfaceUtilities.hpp"
// OCC
#include <Standard.hxx>
#include <gp_Vec.hxx>
#include <gp_Pln.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <TopoDS_Face.hxx>
#include <BRepAdaptor_Surface.hxx>

namespace McCAD::Tools{
  class FaceParameters{
  public:
      FaceParameters(const Standard_Real& precision);
      ~FaceParameters();
  private:
      Standard_Real precision{1.0e-7};
      struct PositionUV{Standard_Real u;
                        Standard_Real v;
                       };
      struct DerivativeUV{gp_Vec u;
                          gp_Vec v;
                         };
      using surfPrmts = std::tuple<gp_Pln, gp_Pnt, gp_Dir, std::array<Standard_Real, 4>>;
  public:
      std::optional<PositionUV> getSurfPositionUV(
              const BRepAdaptor_Surface& surfaceAdaptor, const gp_Pnt& point);
      std::optional<gp_Dir> normalOnFace(const TopoDS_Face& face, const gp_Pnt& point);
      gp_Dir normalOnFace(const DerivativeUV& derivative);
      DerivativeUV calcDerivative(const BRepAdaptor_Surface& surface,
                                  const PositionUV& position);
      surfPrmts genPlSurfParmts(const TopoDS_Face& face);
      void genCylSurfParmts(const TopoDS_Face& face);
      void genTorSurfParmts(const TopoDS_Face& face);
  };
}

#endif
