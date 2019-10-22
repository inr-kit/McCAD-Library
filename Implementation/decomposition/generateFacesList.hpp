#ifndef GENERATEFACESLIST_HPP
#define GENERATEFACESLIST_HPP

//C++
#include <memory>
//McCAD
#include "boundSurface_impl.hpp"
#include "planarSolid_impl.hpp"
#include "cylSolid_impl.hpp"
//OCC
#include <TopoDS_Face.hxx>
#include <Standard.hxx>

namespace McCAD::Decomposition{
    class GenerateFacesList{
    public:
      GenerateFacesList() = default;

      std::vector<std::shared_ptr<Geometry::BoundSurface>> operator()(
              const Geometry::PLSolid& solidObj);
      std::vector<std::shared_ptr<Geometry::BoundSurface>> operator()(
              const Geometry::CYLSolid& solidObj);
      template<typename solidObjType>
      std::shared_ptr<Geometry::BoundSurface> generateSurface(
              const TopoDS_Face& face, Standard_Integer mode = 0,
              Standard_Real& boxSquareLength);
      void mergeSurfaces(std::vector<std::shared_ptr<
                         Geometry::BoundSurface>>& surfacesList);
    };
}

//McCAD
#include "generateFacesList.tpp"

#endif //GENERATEFACESLIST_HPP
