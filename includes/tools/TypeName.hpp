#ifndef SURFACETYPENAME_HPP
#define SURFACETYPENAME_HPP

#include <array>
#include <string>
#include <GeomAbs_SurfaceType.hxx>
#include <GeomAbs_CurveType.hxx>

namespace McCAD::detail{

    std::string toTypeName(
            GeomAbs_SurfaceType surfaceType);

    std::string toTypeName(
            GeomAbs_CurveType curveType);

}

#endif
