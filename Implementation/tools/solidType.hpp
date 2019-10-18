#ifndef SOLIDTYPE_HPP
#define SOLIDTYPE_HPP

//McCAD
#include <Standard.hxx>

namespace McCAD::Tools{
class SolidType{
public:
    SolidType() = default;

    enum customSolidType{
        planarSolid = 0,
        cylindricalSolid,
        sphericalSolid,
        mixedSolid
    };
};
}

#endif // SOLIDTYPE_HPP
