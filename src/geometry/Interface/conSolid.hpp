#ifndef CONSOLID_HPP
#define CONSOLID_HPP

// C++
#include <memory>
// McCAD
#include "solid.hpp"

namespace McCAD::Geometry{
    class CONSolid : public Solid{
    public:
        class Impl;
        CONSolid();
        ~CONSolid();

        Impl* accessCONSImpl() const;
    
    private:
        std::unique_ptr<Impl> pCONSImpl;
    };
}
#endif //CONSOLID_HPP
