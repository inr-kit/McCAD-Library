#ifndef SOLID_IMPL_HPP
#define SOLID_IMPL_HPP

// C++
#include <memory>
#include <vector>
#include <string>
#include <map>
// McCAD
#include "solid.hpp"
#include "tools_impl.hpp"
#include "boundSurface_impl.hpp"
// OCCT
#include <Bnd_OBB.hxx>
#include <Bnd_Box.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TopoDS_Face.hxx>

namespace McCAD::Geometry{
  class Solid::Impl {
  public:
      Impl();
      ~Impl();
  private:
      using facesMap = std::map<std::shared_ptr<BoundSurface>, std::shared_ptr<BoundSurface>>;
  public:
      Bnd_OBB obb;
      Bnd_Box aabb;
      TopoDS_Solid solid;
      TopoDS_Shape solidShape;
      double meshDeflection, boxDiagonalLength, solidVolume;
      bool splitSurface{false}, rejectSolid{false};

      std::vector<std::shared_ptr<BoundSurface>> facesList;
      std::vector<std::shared_ptr<BoundSurface>> planesList;
      std::vector<std::shared_ptr<BoundSurface>> cylindersList;
      std::vector<std::shared_ptr<BoundSurface>> toriList;
      std::vector<std::shared_ptr<BoundSurface>> conesList;
      std::vector<std::shared_ptr<BoundSurface>> assistFacesList;

      std::vector<std::shared_ptr<BoundSurface>> splitFacesList;
      std::vector<std::shared_ptr<BoundSurface>> splitAssistFacesList;
      facesMap assistFacesMap;
      std::vector<std::shared_ptr<BoundSurface>> selectedSplitFacesList;
      std::unique_ptr<TopTools_HSequenceOfShape> splitSolidList;
      std::unique_ptr<TopTools_HSequenceOfShape> rejectedsubSolidsList;

      // Conversion variables
      bool solidIsFillet{false};
      int solidID{ 0 }, compoundID{ 0 }, MCCellID{ 0 };
      gp_Pnt aabbCenter;
      std::vector<std::shared_ptr<BoundSurface>> intersectionList;
      std::vector<std::shared_ptr<BoundSurface>> unionList;
      std::string cellExpr, complimentExpr;
      //using for mcx input file
      std::string cellMcxExpr, complimentMcxExpr;

      void initiate(const TopoDS_Shape& aSolidShape);
      void createBB(double bndBoxGap = 0.0);
      void calcMeshDeflection(double scalingFactor = 100.0);
      void updateEdgesConvexity(double angularTolerance = 1.0e-4 * M_PI,
                                double precision = 1.0e-6);
      void repairSolid(double precision = 1.0e-6, double faceTolerance = 1.0e-8);
      void generateSurfacesList();
      std::unique_ptr<BoundSurface> generateSurface(const TopoDS_Face& face,
                                                    int mode = 0);
      void mergeSurfaces(std::vector<std::unique_ptr<BoundSurface>>& planesList);
      void calcAABBCenter();
      void calcVolume();
  };
}

#endif //SOLID_IMPL_HPP
