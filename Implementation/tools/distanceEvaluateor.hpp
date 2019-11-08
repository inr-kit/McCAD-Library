#ifndef DISTANCEEVALUATOR_HPP
#define DISTANCEEVALUATOR_HPP

// C++
#include <array>
// McCAD
#include <Standard.hxx>
// OCC
#include <TopoDS_Face.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pln.hxx>
#include <gp_Cylinder.hxx>

namespace McCAD::Tools{
  class DistanceEvaluator{
  public:
      DistanceEvaluator() = default;

      Standard_Real operator()(const TopoDS_Face& face, const gp_Pnt& point);
  private:
      Standard_Real distanceToPlane(const gp_Pln& plane, const gp_Pnt& point);
      Standard_Real distanceToCyl(const gp_Cylinder& cylinder, const gp_Pnt& point);
  };
}

#endif //DISTANCEEVALUATOR_HPP
