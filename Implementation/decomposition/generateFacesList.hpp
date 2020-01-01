#ifndef GENERATEFACESLIST_HPP
#define GENERATEFACESLIST_HPP

//C++
#include <memory>
//McCAD
#include "boundSurface_impl.hpp"
#include "planarSolid_impl.hpp"
#include "cylSolid_impl.hpp"
#include "torSolid_impl.hpp"
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
    class FacesListGenerator{
    public:
      FacesListGenerator();
      ~FacesListGenerator();

      using BS = std::shared_ptr<Geometry::BoundSurface>;

      std::vector<BS> operator()(
              Geometry::PLSolid& solidObj);
      std::vector<BS> operator()(
              Geometry::CYLSolid& solidObj);
      std::vector<BS> operator()(
              Geometry::TORSolid& solidObj);
      std::shared_ptr<Geometry::BoundSurface> operator()(
              const TopoDS_Face& face, Standard_Real& boxDiagonalLength,
              Standard_Integer mode = 0);
    };
}

#endif //GENERATEFACESLIST_HPP
