// McCAD
#include "splitsolid_impl.hpp"

void
McCAD::Decomposition::SplitSolid::Impl::splitSolid(const std::shared_ptr<McCAD::Decomposition::BoundSurface>& surface, TopTools_HSequenceOfShape& subSolidsList){
  calculateBoundingBox();
}

void
McCAD::Decomposition::SplitSolid::Impl::calculateBoundingBox(){
  BRepBndLib::Add(solid, bndBox);
  bndBox.SetGap(10.0);
  std::vector<Standard_Real> coordinates(6);
  bndBox.Get(coordinates[0], coordinates[1], coordinates[2], coordinates[3], coordinates[4], coordinates[5]);
  boxSquareLength = sqrt(bndBox.SquareExtent());
  boundingBox = BRepPrimAPI_MakeBox(gp_Pnt(coordinates[0], coordinates[1], coordinates[2]) , gp_Pnt(coordinates[3], coordinates[4], coordinates[5])).Shape();
}
