#ifndef SPLITSOLID_IMPL_HPP
#define SPLITSOLID_IMPL_HPP

// C++
#include <vector>
// McCAD
#include "splitsolid.hpp"
#include "boundSurface_impl.hpp"
// OCC
#include <Handle_TopTools_HSequenceOfShape.hxx>
#include <BRepBndLib.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeHalfSpace.hxx>
#include <gp_Pnt.hxx>
#include <BRepAlgoAPI_Common.hxx>

namespace McCAD::Decomposition{
  class SplitSolid::Impl {
  public:
    Impl() = default;

    Bnd_Box bndBox;
    Standard_Real boxSquareLength;
    TopoDS_Shape boundingBox;

    Standard_Boolean initiate(const TopoDS_Solid& solid, const std::shared_ptr<McCAD::Decomposition::BoundSurface>& surface, Handle_TopTools_HSequenceOfShape& subSolidsList);
    void calculateBoundingBox(const TopoDS_Solid& solid);
    Standard_Boolean split(const TopoDS_Face& splitFace, Handle_TopTools_HSequenceOfShape& subSolidsList);
    void calculatePoints(const TopoDS_Face& splitFace, gp_Pnt& positivePoint, gp_Pnt& negativePoint);

    Standard_Boolean splitWithBoxes(TopoDS_Shape& firstBox, TopoDS_Shape& secondBox);
    void checkRepair(Handle_TopTools_HSequenceOfShape& subSolidsList);

  private:

  };
}

#endif //SPLITSOLID_IMPL_HPP
