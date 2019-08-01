#ifndef SPLITSOLID_IMPL_HPP
#define SPLITSOLID_IMPL_HPP

// C++
#include <vector>
// McCAD
#include "splitsolid.hpp"
#include "boundSurface_impl.hpp"
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

namespace McCAD::Decomposition{
  class SplitSolid::Impl {
  public:
    Impl() = default;

    Bnd_Box bndBox;
    Standard_Real boxSquareLength;
    TopoDS_Shape boundingBox;

    Standard_Boolean initiate(const TopoDS_Solid& solid, const std::shared_ptr<McCAD::Decomposition::BoundSurface>& surface, std::unique_ptr<TopTools_HSequenceOfShape>& subSolidsList);
    void calculateBoundingBox(const TopoDS_Solid& solid);
    Standard_Boolean split(const TopoDS_Solid& solid, const TopoDS_Face& splitFace, std::unique_ptr<TopTools_HSequenceOfShape>& subSolidsList);
    void calculatePoints(const TopoDS_Face& splitFace, gp_Pnt& positivePoint, gp_Pnt& negativePoint);
    Standard_Boolean splitWithBoxes(const TopoDS_Solid& solid, TopoDS_Shape& firstBox, TopoDS_Shape& secondBox, std::unique_ptr<TopTools_HSequenceOfShape>& subSolidsList);
    void checkRepair(std::unique_ptr<TopTools_HSequenceOfShape>& subSolidsList);

  private:

  };
}

#endif //SPLITSOLID_IMPL_HPP
