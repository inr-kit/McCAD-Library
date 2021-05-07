#ifndef CONVERSIONWRITER_HPP
#define CONVERSIONWRITER_HPP

// C++
#include <string>
#include <vector>
#include <memory>
// McCAD
#include "solid_impl.hpp"
#include "voidCell.hpp"
#include "inputconfig.hpp"
// OCC
#include <Standard.hxx>

namespace McCAD::Conversion{
    class Writer {
    public:
        Writer(const IO::InputConfig& inputConfig);
        ~Writer();
    private:
        using solidsList = std::vector<std::shared_ptr<Geometry::Solid>>;
    public:
        IO::InputConfig inputConfig;
        std::string MCcode;
        void operator()(const solidsList& solidObjList,
                        const std::shared_ptr<VoidCell>& voidCell);
    };
}

#endif // CONVERSIONWRITER_HPP
