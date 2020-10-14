#ifndef CONVERSION_HPP
#define CONVERSION_HPP

// C++
#include <memory>
// McCAD
#include "inputconfig.hpp"
#include "inputdata.hpp"

namespace McCAD::Conversion{
    class Convert{
    private:
        class Impl;
    public:
        Convert(const IO::InputConfig& inputConfig,
                const General::InputData& inputData);
        ~Convert();

        Impl* accessCImpl() const;
    private:
        std::unique_ptr<Impl> pCImpl;
    };
}
#endif //CONVERSION_HPP
