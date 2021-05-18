#ifndef SOLID_IMPL_HPP
#define SOLID_IMPL_HPP

// C++
#include <memory>
#include <vector>
#include <string>
// McCAD
#include "solid.hpp"
#include "tools_impl.hpp"
#include "boundSurface_impl.hpp"
// OCC
#include <Standard.hxx>
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

    std::unique_ptr<Tools::Preprocessor> preproc;
    
    Bnd_OBB obb;
    Bnd_Box aabb;
    TopoDS_Solid solid;
    TopoDS_Shape solidShape;
    Standard_Real meshDeflection, boxDiagonalLength, solidVolume;
    Standard_Boolean splitSurface = Standard_False;
    
    std::vector<std::shared_ptr<BoundSurface>> facesList;
    std::vector<std::shared_ptr<BoundSurface>> planesList;
    std::vector<std::shared_ptr<BoundSurface>> cylindersList;
    std::vector<std::shared_ptr<BoundSurface>> toriList;

    std::vector<std::shared_ptr<BoundSurface>> splitFacesList;
    std::vector<std::shared_ptr<BoundSurface>> selectedSplitFacesList;
    std::unique_ptr<TopTools_HSequenceOfShape> splitSolidList;
    std::unique_ptr<TopTools_HSequenceOfShape> rejectedsubSolidsList;

    // Conversion variables
    Standard_Integer solidID{0}, compoundID{0};
    gp_Pnt aabbCenter;
    std::vector<std::shared_ptr<BoundSurface>> intersectionList;
    std::vector<std::shared_ptr<BoundSurface>> unionList;
    std::string cellExpr, complimentExpr;

    void initiate(const TopoDS_Shape& aSolidShape);
    void createOBB(Standard_Real bndBoxGap = 0);
    void calcMeshDeflection(Standard_Real scalingFactor = 100);
    void updateEdgesConvexity(const Standard_Real& angleTolerance = 1.0e-4);
    void repairSolid();
    void generateSurfacesList();
    std::unique_ptr<BoundSurface> generateSurface(const TopoDS_Face& face,
                                                  Standard_Integer mode = 0);
    void mergeSurfaces(std::vector<std::unique_ptr<BoundSurface>>& planesList);
    void calcAABBCenter();
    void calcVolume();
  };
}

#endif //SOLID_IMPL_HPP
