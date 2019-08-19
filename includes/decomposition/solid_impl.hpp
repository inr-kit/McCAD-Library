#ifndef SOLID_IMPL_HPP
#define SOLID_IMPL_HPP

// C++
#include <vector>
#include <memory>
#include <assert.h>
#include <sstream>
#include <tuple>
// McCAD
#include "decomposeSolid.hpp"
#include "tools_impl.hpp"
#include "edge_impl.hpp"
#include "surface_impl.hpp"
#include "boundSurface_impl.hpp"
#include "boundSurfacePlane_impl.hpp"
#include "splitsurfaces_impl.hpp"
#include "splitsolid_impl.hpp"
#include <Standard.hxx>
// OCC
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <Geom_Curve.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <STEPControl_Writer.hxx>

namespace McCAD::Decomposition{
  class Solid::Impl {
  public:
    Impl();
    ~Impl();

    Tools::Preprocessor preproc;
    
    TopoDS_Solid solid;
    TopoDS_Shape solidShape;
    Standard_Boolean isTorus = Standard_False;
    Standard_Boolean isSpline = Standard_False;
    Standard_Real meshDeflection;
    Standard_Real boxSquareLength;
    Standard_Boolean splitSurface = Standard_False;
    std::vector<std::shared_ptr<BoundSurface>> facesList;
    std::vector<std::shared_ptr<BoundSurface>> splitFacesList;
    std::unique_ptr<TopTools_HSequenceOfShape> splitSolidList;
    std::unique_ptr<TopTools_HSequenceOfShape> rejectedsubSolidsList;

    void initiate(const TopoDS_Shape& aSolidShape);
    void updateEdgesConvexity(const Standard_Real& angleTolerance = 1.0e-4);
    void repairSolid();
    void generateSurfacesList();
    std::unique_ptr<BoundSurface> generateSurface(const TopoDS_Face& face, Standard_Integer mode = 0);
    void mergeSurfaces(std::vector<std::unique_ptr<BoundSurface>>& planesList);

  private:

  };
}

#endif //SOLID_IMPL_HPP
