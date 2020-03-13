#ifndef ASSISTSURFACEGERAOR_HPP
#define ASSISTSURFACEGERAOR_HPP

// C++
#include <memory>
// McCAD
#include <Standard.hxx>
#include "solid_impl.hpp"
#include "torSolid_impl.hpp"
#include "cylSolid_impl.hpp"

// OCC


namespace McCAD::Decomposition{
  class AssistSurfaceGenerator{
  public:
    void operator()(Geometry::TORSolid& solidObj,
                    Standard_Real angleTolerance = 0.785); //0.785 >> 45 degrees
    void operator()(Geometry::CYLSolid& solidObj);
  };
}

#endif //ASSISTSURFACEGERAOR_HPP
