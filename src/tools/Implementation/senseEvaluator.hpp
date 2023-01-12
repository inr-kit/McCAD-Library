#ifndef SENSEEVALUATOR_HPP
#define SENSEEVALUATOR_HPP

// C++
#include <optional>
// OCCT
#include <TopoDS_Face.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pln.hxx>
#include <gp_Cylinder.hxx>
#include <gp_Cone.hxx>

namespace McCAD::Tools{
  class SenseEvaluator{
  public:
      SenseEvaluator() = default;

      std::optional<double> operator()(const TopoDS_Face& face,
                                       const gp_Pnt& point);
      double senseRelativeToPlane(const gp_Pln& plane, const gp_Pnt& point);
      double senseRelativeToCyl(const gp_Cylinder& cylinder, const gp_Pnt& point);
      double senseRelativeToCone(const gp_Cone& cone, const gp_Pnt& point);
  };
}

#endif //SENSEEVALUATOR_HPP
