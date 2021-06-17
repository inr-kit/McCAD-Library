// C++
#include <algorithm>
// McCAD
#include "splitSurfacesGenerator.hpp"
#include "SurfaceUtilities.hpp"
// OCC
#include <GeomAbs_SurfaceType.hxx>

McCAD::Decomposition::SplitSurfaceGenerator::SplitSurfaceGenerator(){
}

McCAD::Decomposition::SplitSurfaceGenerator::~SplitSurfaceGenerator(){
}

void
McCAD::Decomposition::SplitSurfaceGenerator::generatePlaneOnEdge(
        const TopoDS_Face& firstFace, const TopoDS_Face& secondFace,
        std::shared_ptr<Geometry::Edge> edge){

}

