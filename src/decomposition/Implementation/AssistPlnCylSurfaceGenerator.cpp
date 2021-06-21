//C++

// McCAD
#include "AssistPlnCylSurfaceGenerator.hpp"
//OCC
#include <Standard.hxx>


McCAD::Decomposition::AssistPlnCylSurfaceGenerator::AssistPlnCylSurfaceGenerator(
        const IO::InputConfig& inputConfig) : inputConfig{inputConfig}{
}

McCAD::Decomposition::AssistPlnCylSurfaceGenerator::~AssistPlnCylSurfaceGenerator(){
}

void
McCAD::Decomposition::AssistPlnCylSurfaceGenerator::operator()(
        Geometry::CYLSolid& solidObj){
}
