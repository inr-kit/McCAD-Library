#ifndef TRIANGLECOLLISION_HPP
#define TRIANGLECOLLISION_HPP

//C++
#include <memory>
//McCAD
#include <Standard.hxx>
#include "meshtriangle_impl.hpp"
//OCC

namespace McCAD::Decomposition{
    class TriangleCollision{
    public:
      TriangleCollision();
      ~TriangleCollision();

      template<typename surfaceType>
      Standard_Boolean triangleCollision(const TopoDS_Face& extendedFace,
                                         const MeshTriangle& aTriangle,
                                         Standard_Integer& aSide,
                                         Standard_Real tolerance = 1.0e-2,
                                         Standard_Real tolerance2 = 1.0e-3);
    };
}

#include "triangleCollision.tpp"

#endif //TRIANGLECOLLISION_HPP
