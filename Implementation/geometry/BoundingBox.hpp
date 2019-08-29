#ifndef BOUNDINGBOX_HPP
#define BOUNDINGBOX_HPP

// McCAD
#include "Coord3D.hpp"
#include "Coordinates.ipp"
// OCC
#include <TopoDS_Solid.hxx>
#include <algorithm>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>

namespace McCAD::Geometry{

    class BoundingBox{

    public:
        BoundingBox() = default;
        BoundingBox(const Coord3D& minima,
                    const Coord3D& maxima);

        const Coord3D& minima() const;
        const Coord3D& maxima() const;

        const coord_type& getGap() const;
        bool setGap(coord_type gap);
        void resetGap();

        coord_type minSize() const;
        coord_type maxSize() const;

        coord_type diagonal() const;

    private:
        Coord3D minima_;
        Coord3D maxima_;
        coord_type gap_;

    };

    namespace detail{
        BoundingBox makeBoundingBox(const TopoDS_Solid& solid);
    }

}

#endif
