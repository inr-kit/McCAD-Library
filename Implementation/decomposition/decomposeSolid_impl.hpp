#ifndef DECOMPOSESOLID_IMPL_HPP
#define DECOMPOSESOLID_IMPL_HPP

// C++
#include <vector>
#include <memory>
#include <assert.h>
#include <sstream>
#include <tuple>
// McCAD
#include "decomposeSolid.hpp"
#include "tools_impl.hpp"
#include "solid_impl.hpp"
#include "boundSurface_impl.hpp"
#include "splitsurfaces_impl.hpp"
#include "splitsolid_impl.hpp"
#include <Standard.hxx>
#include "SurfaceUtilities.hpp"
#include "CurveUtilities.hpp"
#include "PlaneFuser.hpp"
#include "FaceParameters.hpp"
#include "ShapeView.hpp"
#include "PlaneFuser.hpp"
#include "FaceParameters.hpp"
// OCC
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
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
  class DecomposeSolid::Impl {
  public:
    Impl();
    ~Impl();

    Tools::Preprocessor preproc;
    SplitSurfaces splitSurfaces;
    SplitSolid splitSolid;

    Standard_Integer recurrenceDepth = 0;
    
    Standard_Boolean perform(std::unique_ptr<Geometry::Solid>& solid);
    void judgeDecomposeSurfaces(std::unique_ptr<Geometry::Solid>& solid);
    void judgeThroughConcaveEdges(std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList);
    void generateAssistingSurfaces();
    Standard_Boolean selectSplitSurface(std::unique_ptr<Geometry::Solid>& solid);
  };
}

#endif //DECOMPOSESOLID_IMPL_HPP
