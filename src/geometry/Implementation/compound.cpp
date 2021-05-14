// McCAD
#include "compound.hpp"

McCAD::Geometry::Impl::Compound::Compound(const TopoDS_Shape& shape,
                                          const TCollection_ExtendedString& name):
    compoundShape{shape}, compoundName{name},
    acceptedInputShapesList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedInputShapesList{std::make_unique<TopTools_HSequenceOfShape>()},
    subSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedsubSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}{}

McCAD::Geometry::Impl::Compound::~Compound(){}
