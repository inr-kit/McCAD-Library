#ifndef SURFACEOBJCREATOR_HPP
#define SURFACEOBJCREATOR_HPP

//C++
#include <memory>
//McCAD
#include "boundSurface_impl.hpp"
//OCC
#include <TopoDS_Face.hxx>
#include <Standard.hxx>

namespace McCAD::Decomposition{
    class SurfaceObjCreator{
    public:
      SurfaceObjCreator() = default;
      
      std::shared_ptr<McCAD::Geometry::BoundSurface> operator()(
              const TopoDS_Face& face, Standard_Real& boxDiagonalLength,
              Standard_Integer mode = 0);
    };
}

#endif //SURFACEOBJCREATOR_HPP
