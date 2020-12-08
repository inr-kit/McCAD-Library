#ifndef BVHCREATOR_HPP
#define BVHCREATOR_HPP

// C++
#include <memory>
// MOAB
#include <moab/Core.hpp>
// OCC
#include <TopTools_HSequenceOfShape.hxx>
// McCAD
#include "inputconfig.hpp"

namespace McCAD::Conversion{
    class BVHCreator {
    public:
        BVHCreator(const IO::InputConfig& inputConfig,
                   const std::shared_ptr<TopTools_HSequenceOfShape> inputSolidsList);
        ~BVHCreator();

        std::shared_ptr<moab::Core::Interface> moabImpl;
    };
}
#endif //BVHCREATOR_HPP
