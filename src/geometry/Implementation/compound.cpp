// McCAD
#include "compound.hpp"

/** ********************************************************************
* @brief   A constructor for a compound object.
* @param   shape is a shape loaded from the input STEP file(s).
* @param   name is a OCCT string object of the name of the solid.
* @date    31/12/2020
* @author  Moataz Harb
* **********************************************************************/
McCAD::Geometry::Impl::Compound::Compound(const TopoDS_Shape& shape,
                                          const TCollection_ExtendedString& name):
    compoundShape{shape}, compoundName{name},
    acceptedInputShapesList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedInputShapesList{std::make_unique<TopTools_HSequenceOfShape>()},
    subSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedsubSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}{}

McCAD::Geometry::Impl::Compound::~Compound(){}
