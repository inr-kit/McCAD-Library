#ifndef SOLIDTYPE_HPP
#define SOLIDTYPE_HPP


namespace McCAD::Tools{
    class SolidType{
        public:
            SolidType() = default;

            enum customSolidType{
                planar = 1,
                cylindrical,
                toroidal,
                mixed,
                conical,
                spherical,
                unKnown
            };
    };
}

#endif // SOLIDTYPE_HPP
