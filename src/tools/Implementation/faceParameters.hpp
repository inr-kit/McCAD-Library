#ifndef FACEPARAMETERS_HPP
#define FACEPARAMETERS_HPP

// C++
#include <array>
#include <vector>
#include <tuple>
#include <optional>
// McCAD
#include "SurfaceUtilities.hpp"
// OCCT
#include <gp_Pln.hxx>
#include <gp_Cylinder.hxx>
#include <gp_Torus.hxx>
#include <gp_Vec.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <TopoDS_Face.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <gp_Cone.hxx>

namespace McCAD::Tools{
  class FaceParameters{
  public:
      FaceParameters();
      FaceParameters(const double& precision);
      FaceParameters(const double& precision, const double& scalingFactor);
      ~FaceParameters();
  private:
      double precision{1.0e-6}, scalingFactor{1.0};
      struct PositionUV{double u;
                        double v;
                       };
      struct DerivativeUV{gp_Vec u;
                          gp_Vec v;
                         };
      using planePrmts = std::tuple<gp_Pln, gp_Pnt, gp_Dir, std::array<double, 4>>;
      using cylinderPrmts = std::tuple<gp_Cylinder, gp_Pnt, gp_Dir, std::array<double, 10>,
                                       double, int>;
      using torusPrmts = std::tuple<gp_Torus, gp_Pnt, gp_Dir, std::vector<double>,
                                    double, double, int>;
      using conePrmts = std::tuple<gp_Cone, gp_Pnt, gp_Dir, gp_Pnt, double, std::array<double, 10>,
                                   double, int>;
  public:
      std::optional<PositionUV> getSurfPositionUV(
              const BRepAdaptor_Surface& surfaceAdaptor, const gp_Pnt& point);
      std::optional<gp_Dir> normalOnFace(const TopoDS_Face& face, const gp_Pnt& point);
      gp_Dir normalOnFace(const DerivativeUV& derivative);
      DerivativeUV calcDerivative(const BRepAdaptor_Surface& surface,
                                  const PositionUV& position);
      planePrmts genPlSurfParmts(const TopoDS_Face& face);
      cylinderPrmts genCylSurfParmts(const TopoDS_Face& face);
      double getRadian(const TopoDS_Face& aFace);
      torusPrmts genTorSurfParmts(const TopoDS_Face& face);
      conePrmts genConeSurfParmts(const TopoDS_Face& face);
  };
}

#endif
