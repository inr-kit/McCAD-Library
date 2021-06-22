#ifndef EDGESCOMPARATOR_HPP
#define EDGESCOMPARATOR_HPP

// C++
#include <math.h>
// OCC
#include <Standard.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepAdaptor_Curve.hxx>

namespace McCAD::Tools{
  class EdgesComparator{
  public:
      EdgesComparator();
      EdgesComparator(const Standard_Real& angularTolerance,
                      const Standard_Real& distanceTolerance,
                      const Standard_Real& precision);
      ~EdgesComparator();
  private:
      Standard_Real angularTolerance{1.0e-4 * M_PI},
                    distanceTolerance{1.0e-5},
                    precision{1.0e-7};
  public:
      Standard_Boolean operator()(const TopoDS_Edge& firstEdge,
                                  const TopoDS_Edge& secondEdge);
      Standard_Boolean compareLines(const BRepAdaptor_Curve& firstEdge,
                                    const BRepAdaptor_Curve& secondEdge);
      Standard_Boolean compareCircles(const BRepAdaptor_Curve& firstEdge,
                                      const BRepAdaptor_Curve& secondEdge);
      Standard_Boolean compareEllipses(const BRepAdaptor_Curve& firstEdge,
                                       const BRepAdaptor_Curve& secondEdge);
  };
}

#endif //EDGESCOMPARATOR_HPP
