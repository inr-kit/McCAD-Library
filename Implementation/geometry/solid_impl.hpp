#ifndef SOLID_IMPL_HPP
#define SOLID_IMPL_HPP

// C++
#include <vector>
#include <memory>
#include <filesystem>
// McCAD
#include "solid.hpp"
#include "tools_impl.hpp"
#include "boundSurface_impl.hpp"
#include "boundSurfacePlane_impl.hpp"
#include <Standard.hxx>
#include "SurfaceUtilities.hpp"
#include "CurveUtilities.hpp"
#include "PlaneFuser.hpp"
#include "FaceParameters.hpp"
#include "ShapeView.hpp"
// OCC
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>
#include <Geom_Curve.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <Bnd_OBB.hxx>
#include <STEPControl_Writer.hxx>

namespace McCAD::Geometry{
  class Solid::Impl {
  public:
    Impl();
    ~Impl();

    std::unique_ptr<Tools::Preprocessor> preproc;
    
    Bnd_OBB obb;
    TopoDS_Solid solid;
    TopoDS_Shape solidShape;
    Standard_Real meshDeflection;
    Standard_Real boxSquareLength;
    Standard_Boolean splitSurface = Standard_False;
    
    std::vector<std::shared_ptr<BoundSurface>> facesList;
    std::vector<std::shared_ptr<BoundSurface>> splitFacesList;
    std::vector<std::shared_ptr<BoundSurface>> selectedSplitFacesList;
    std::unique_ptr<TopTools_HSequenceOfShape> splitSolidList;
    std::unique_ptr<TopTools_HSequenceOfShape> rejectedsubSolidsList;

    void initiate(const TopoDS_Shape& aSolidShape);
    void createOBB(Standard_Real bndBoxGap = 0);
    void calcMeshDeflection(Standard_Real scalingFactor = 100);
    void updateEdgesConvexity(const Standard_Real& angleTolerance = 1.0e-4);
    void repairSolid();
    void generateSurfacesList();
    std::unique_ptr<BoundSurface> generateSurface(const TopoDS_Face& face,
                                                  Standard_Integer mode = 0);
    void mergeSurfaces(std::vector<std::unique_ptr<BoundSurface>>& planesList);
  };
}

#endif //SOLID_IMPL_HPP
