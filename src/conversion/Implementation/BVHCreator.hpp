#ifndef BVHCREATOR_HPP
#define BVHCREATOR_HPP

// C++
#include <memory>
// MOAB
#include "moab/Core.hpp"

namespace McCAD::Conversion{
    class BVHCreator {
    public:
        BVHCreator(const std::string& inputFileName);
        ~BVHCreator();

        std::shared_ptr<moab::Core::Interface> moabImpl;
    };
}
#endif //BVHCREATOR_HPP
