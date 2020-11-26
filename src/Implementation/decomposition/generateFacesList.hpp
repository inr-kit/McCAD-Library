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
      using BS = std::shared_ptr<Geometry::BoundSurface>;

    private:
      std::vector<BS> facesList;
      std::vector<BS> planesList;
      std::vector<BS> cylindersList;
      std::vector<BS> toriList;

    public:
      template <typename solidObjType>
      std::vector<BS> operator()(
             solidObjType& solidObj);
      template <typename solidObjType>
      void addListsToSolidObj(solidObjType& solidObj);

      void mergePlanesList(Standard_Real& boxDiagonalLength);
      void mergeCylindersList(Standard_Real& boxDiagonalLength);
      void mergeToriList(Standard_Real& boxDiagonalLength);
    };
}

//McCAD
#include "generateFacesList.tpp"

#endif //GENERATEFACESLIST_HPP
