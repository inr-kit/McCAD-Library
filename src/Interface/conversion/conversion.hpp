#ifndef CONVERSION_HPP
#define CONVERSION_HPP

// C++
#include <memory>
// McCAD
#include "inputconfig.hpp"

namespace McCAD::Conversion{
    class Convert{
    private:
        class Impl;
    public:
        Convert(const IO::InputConfig& inputConfig);
        ~Convert();

        Impl* accessCImpl() const;
    private:
        std::unique_ptr<Impl> pCImpl;
    };
}
#endif //CONVERSION_HPP
