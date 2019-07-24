#ifndef SURFACE_IMPL_HPP
#define SURFACE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "surface.hpp"
// OCC
#include <TopoDS_Face.hxx>

namespace McCAD::Decomposition{
  class Surface::Impl {
  public:
    Impl() = default;

    TopoDS_Face face;
    std::string surfaceType;
    Standard_Integer surfaceNumber;
    McCAD::Tools::Preprocessor preproc;
    void initiate(const TopoDS_Face& aFace);
    Standard_Boolean isEqual(std::unique_ptr<Surface::Impl>& that);

  private:

  };
}

#endif //SURFACE_IMPL_HPP
