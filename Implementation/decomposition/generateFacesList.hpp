#ifndef GENERATEFACESLIST_HPP
#define GENERATEFACESLIST_HPP

//C++
#include <memory>
//McCAD
#include "boundSurface_impl.hpp"
#include "boundSurfacePlane_impl.hpp"
#include "boundSurfaceCyl_impl.hpp"
#include "planarSolid_impl.hpp"
#include "cylSolid_impl.hpp"
#include "SurfaceUtilities.hpp"
#include "CurveUtilities.hpp"
#include "PlaneFuser.hpp"
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
              const Geometry::PLSolid& solidObj);
      std::vector<BS> operator()(
              const Geometry::CYLSolid& solidObj);
      template<typename solidObjType>
      BS generateSurface(const TopoDS_Face& face,
                         Standard_Real& boxSquareLength,
                         Standard_Integer mode = 0);
      void mergeSurfaces(std::vector<BS>& surfacesList,
                         Standard_Real& boxSquareLength);
    };
}

#endif //GENERATEFACESLIST_HPP
