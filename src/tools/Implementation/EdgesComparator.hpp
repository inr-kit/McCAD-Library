#ifndef EDGESCOMPARATOR_HPP
#define EDGESCOMPARATOR_HPP

// C++
#include <math.h>
// OCCT
#include <TopoDS_Edge.hxx>
#include <BRepAdaptor_Curve.hxx>

namespace McCAD::Tools{
  class EdgesComparator{
  public:
      EdgesComparator();
      EdgesComparator(const double& angularTolerance,
                      const double& distanceTolerance,
                      const double& precision);
      ~EdgesComparator();
  private:
      double angularTolerance{1.0e-4 * M_PI}, distanceTolerance{1.0e-6},
             precision{1.0e-6};
  public:
      bool operator()(const TopoDS_Edge& firstEdge, 
                      const TopoDS_Edge& secondEdge);
      bool compareLines(const BRepAdaptor_Curve& firstEdge, 
                        const BRepAdaptor_Curve& secondEdge);
      bool compareCircles(const BRepAdaptor_Curve& firstEdge,
                          const BRepAdaptor_Curve& secondEdge);
      bool compareEllipses(const BRepAdaptor_Curve& firstEdge,
                           const BRepAdaptor_Curve& secondEdge);
      bool compareBSplines(const BRepAdaptor_Curve& firstEdge,
                           const BRepAdaptor_Curve& secondEdge);
  };
}

#endif //EDGESCOMPARATOR_HPP
