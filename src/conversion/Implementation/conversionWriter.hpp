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

namespace McCAD::Conversion{
    class Writer {
    public:
        Writer(const IO::InputConfig& inputConfig);
        ~Writer();
    private:
        using solidsList = std::vector<std::shared_ptr<Geometry::Solid>>;
    public:
        std::string MCcode;
        std::string MCOutputFileName;
        Standard_Real precision;
        int startCellNum, startSurfNum;
        void operator()(const solidsList& solidObjList);
        void operator()(const solidsList& solidObjList,
                        const std::shared_ptr<VoidCell>& voidCell);
    };
}

#endif // CONVERSIONWRITER_HPP
