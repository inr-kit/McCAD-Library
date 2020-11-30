#ifndef BOUNDINGBOX_HPP
#define BOUNDINGBOX_HPP

// McCAD
#include "Coordinates.ipp"
#include "common.ipp"

// OCC
#include <TopoDS_Solid.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>

// C++
#include <algorithm>

namespace McCAD::Geometry{

    class BoundingBox{

    public:
        BoundingBox() = default;
        BoundingBox(const Coord3D& minima, const Coord3D& maxima);

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
  };
}

#endif //BOUNDINGBOX_HPP
