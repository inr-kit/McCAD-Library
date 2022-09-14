#ifndef SURFACE_IMPL_HPP
#define SURFACE_IMPL_HPP

// C++
#include <string>
#include <vector>
// McCAD
#include "surface.hpp"
#include "tools_impl.hpp"
// OCCT
#include <TopoDS_Face.hxx>
#include <gp_Pln.hxx>
#include <gp_Cylinder.hxx>
#include <gp_Torus.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Cone.hxx>
#include <gp_Cone.hxx>

namespace McCAD::Geometry{
    class Surface::Impl {
        public:
            Impl() = default;
            TopoDS_Face face, extendedFace;
            std::string surfaceType;
            int surfaceNumber;
            bool splitSurface{ false }, 
                 hasAssistSurface{ false }, 
                 isAssistSurface{false};
            int numberCollidingSurfaces{ 0 }, 
                numberCollidingCurvedSurfaces{ 0 }, 
                throughConcaveEdges{ 0 }, 
                internalLoops{ 0 }, 
                repeatedSurface{ 0 };

            void initiate(const TopoDS_Face& aFace);
            void countInternalLoops();

            // Conversion variables.
            bool updated{false};
            std::string surfSymb;
            int uniqueID;
            std::string surfExpr;
            // Shared between surfaces.
            signed int surfSense;
            gp_Pnt location;
            gp_Dir symmetryAxis;
            std::vector<double> surfParameters;
            // Plane
            gp_Pln plane;
            gp_Dir normal;
            // Cylinder
            gp_Cylinder cylinder;
            double radius;
            // Torus
            gp_Torus torus;
            double minorRadius, majorRadius;
            // Cone
            gp_Cone cone;
            double refRadius;
    };
}

#endif //SURFACE_IMPL_HPP
