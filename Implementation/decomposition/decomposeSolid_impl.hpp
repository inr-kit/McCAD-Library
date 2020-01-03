#ifndef DECOMPOSESOLID_IMPL_HPP
#define DECOMPOSESOLID_IMPL_HPP

// C++
#include <vector>
#include <memory>
#include <assert.h>
#include <sstream>
#include <tuple>
#include <variant>
// McCAD
#include "decomposeSolid.hpp"
#include "preprocessor.hpp"
#include "tools_impl.hpp"
#include "solid_impl.hpp"
#include "cylSolid_impl.hpp"
#include "planarSolid_impl.hpp"
#include "torSolid_impl.hpp"
#include "boundSurface_impl.hpp"
#include "splitsurfaces_impl.hpp"
#include "splitsolid_impl.hpp"
#include <Standard.hxx>
#include "SurfaceUtilities.hpp"
#include "CurveUtilities.hpp"
#include "FaceParameters.hpp"
#include "ShapeView.hpp"
#include "FaceParameters.hpp"
#include "SolidType.hpp"
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
  class DecomposeSolid::Impl{
  public:
    Impl();
    Impl(Standard_Integer recurrenceDepth);
    ~Impl();
    
    Standard_Boolean operator()(std::shared_ptr<Geometry::PLSolid>& solidObj);
    Standard_Boolean operator()(std::shared_ptr<Geometry::CYLSolid>& solidObj);
    Standard_Boolean operator()(std::shared_ptr<Geometry::TORSolid>& solidObj);

    Tools::SolidType solidType;
    Standard_Integer recurrenceDepth;
    SplitSurfaces splitSurfaces;
    SplitSolid splitSolid;

    static Standard_Boolean throughNoBoundarySurfaces(
            const std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList);
    static Standard_Boolean planeSplitOnlyPlane(
            const std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList);
    Standard_Boolean perform(Geometry::Solid::Impl& solidImpl);

    void splitSubsolid(Geometry::Solid::Impl& solidImpl,
                       Standard_Integer& index);

    Standard_Boolean selectSplitSurface(Geometry::Solid::Impl& solidImpl);
    void extractSolids(Geometry::Solid::Impl& solidImpl,
                       const Geometry::Solid::Impl& subSolidImpl,
                       Standard_Integer& i);
  };
}

#endif //DECOMPOSESOLID_IMPL_HPP
