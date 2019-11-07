#ifndef GENERATEFACESLIST_HPP
#define GENERATEFACESLIST_HPP

//C++
#include <memory>
//McCAD
#include "boundSurface_impl.hpp"
#include "boundSurface_impl.hpp"
#include "boundSurfaceCyl_impl.hpp"
#include "planarSolid_impl.hpp"
#include "cylSolid_impl.hpp"
#include "tools_impl.hpp"
#include "SurfaceUtilities.hpp"
#include "CurveUtilities.hpp"
#include "surfacesFuser.hpp"
#include "FaceParameters.hpp"
#include "ShapeView.hpp"
//OCC
#include <TopoDS_Face.hxx>
#include <Standard.hxx>

namespace McCAD::Decomposition{
    class GenerateFacesList{
    public:
      GenerateFacesList();
      ~GenerateFacesList();

      std::unique_ptr<Tools::Preprocessor> preproc;

      std::vector<std::shared_ptr<Geometry::BoundSurface>> operator()(
              const Geometry::PLSolid& solidObj);
      std::vector<std::shared_ptr<Geometry::BoundSurface>> operator()(
              const Geometry::CYLSolid& solidObj);
      std::shared_ptr<Geometry::BoundSurface> operator()(
              const TopoDS_Face& face, Standard_Real& boxSquareLength,
              Standard_Integer mode = 0);
      template<typename solidObjType>
      std::shared_ptr<Geometry::BoundSurface> generateSurface(
              const TopoDS_Face& face, Standard_Real& boxSquareLength,
              Standard_Integer mode = 0);
      void mergeSurfaces(
              std::vector<std::shared_ptr<Geometry::BoundSurface>>& surfacesList,
              Standard_Real& boxSquareLength);
    };
}

#endif //GENERATEFACESLIST_HPP
