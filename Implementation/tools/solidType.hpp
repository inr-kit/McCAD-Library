#ifndef SOLIDTYPE_HPP
#define SOLIDTYPE_HPP

//McCAD
#include <Standard.hxx>

namespace McCAD::Tools{
class SolidType{
public:
    SolidType() = default;

    enum customSolidType{
        planar = 1,
        cylindrical,
        toroidal,
        spherical,
        mixed
    };
};
}

#endif // SOLIDTYPE_HPP
