#ifndef GENERATEFACESLIST_HPP
#define GENERATEFACESLIST_HPP

//C++
#include <memory>
//McCAD
#include "boundSurface_impl.hpp"
#include "boundSurfacePlane_impl.hpp"
//OCC


namespace McCAD::Decomposition{
    class GenerateFacesList{
    public:
      GenerateFacesList() = default;

      template<typename solidObjType>
      std::vector<std::shared_ptr<BoundSurface>> operator(const solidObjType& solidObj);
      std::unique_ptr<McCAD::Geometry::BoundSurface> generateSurface(
              const TopoDS_Face& face, Standard_Integer mode);
    };
}

//McCAD
#include "generateFacesList.tpp"

#endif //GENERATEFACESLIST_HPP
