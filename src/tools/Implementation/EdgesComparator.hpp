#ifndef EDGESCOMPARATOR_HPP
#define EDGESCOMPARATOR_HPP

// McCAD
#include <Standard.hxx>
// OCC
#include <TopoDS_Edge.hxx>

namespace McCAD::Tools{
  class EdgesComparator{
  public:
      EdgesComparator();
      EdgesComparator(const Standard_Real& angularTolerance,
                      const Standard_Real& distanceTolerance);
      ~EdgesComparator();
  private:
      Standard_Real angularTolerance{1.0e-4 * M_PI}, distanceTolerance{1.0e-5};
  public:
      Standard_Boolean operator()(const TopoDS_Edge& firstEdge,
                                  const TopoDS_Edge& secondEdge);
  };
}

#endif //EDGESCOMPARATOR_HPP
