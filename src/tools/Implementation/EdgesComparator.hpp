#ifndef EDGESCOMPARATOR_HPP
#define EDGESCOMPARATOR_HPP

// McCAD
#include <Standard.hxx>
// OCC
#include <TopoDS_Edge.hxx>

namespace McCAD::Tools{
  class EdgesComparator{
  public:
      Standard_Boolean operator()(const TopoDS_Edge& firstEdge,
                                  const TopoDS_Edge& secondEdge,
                                  Standard_Real angularTolerance = 1.0e-3 * M_PI,
                                  Standard_Real distanceTolerance = 1.0e-5);
  };
}

#endif //EDGESCOMPARATOR_HPP
