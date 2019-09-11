#ifndef SPLITSOLID_IMPL_HPP
#define SPLITSOLID_IMPL_HPP

// C++
#include <array>
#include <filesystem>
#include <string>
// McCAD
#include "splitsolid.hpp"
#include "solid_impl.hpp"
#include "boundSurface_impl.hpp"
#include "ShapeView.hpp"
// OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <BRepBndLib.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeHalfSpace.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <gp_Pln.hxx>
#include <gp_Ax3.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <STEPControl_Writer.hxx>
#include <GProp_GProps.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <ShapeFix_Solid.hxx>
#include <TopoDS_Shell.hxx>
#include <Bnd_OBB.hxx>
#include <gp_Trsf.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <TopLoc_Location.hxx>

namespace McCAD::Decomposition{
  class SplitSolid::Impl {
  public:
    Impl() = default;

    Bnd_OBB bndBox;
    Standard_Real boxSquareLength;
    TopoDS_Shape boundingBox;

    Standard_Boolean perform(std::unique_ptr<Geometry::Solid>& solid,
			     Standard_Integer indexSplitSurface = 0);
    void createOBBSolid(const Bnd_OBB& OBB, TopoDS_Solid& solid);
    Standard_Boolean split(const TopoDS_Solid& solid,
			   const TopoDS_Face& splitFace,
			   std::unique_ptr<TopTools_HSequenceOfShape>& subSolidsList);
    void calculatePoints(const TopoDS_Face& splitFace,
			 gp_Pnt& positivePoint,
			 gp_Pnt& negativePoint);
    Standard_Boolean splitWithBoxes(const TopoDS_Solid& solid,
				    TopoDS_Shape& firstBox,
				    TopoDS_Shape& secondBox,
				    std::unique_ptr<TopTools_HSequenceOfShape>& subSolidsList);
    Standard_Boolean checkRepair(std::unique_ptr<TopTools_HSequenceOfShape>& subSolidsList,
				 Standard_Real tolerance = 1.0e-4);
    Standard_Boolean rebuildSolidFromShell(const TopoDS_Shape& solid,
					   TopoDS_Solid& resultSolid,
					   Standard_Real tolerance = 1.0e-2,
					   Standard_Real tolerance2 = 1.0e-4);
  };
}

#endif //SPLITSOLID_IMPL_HPP
