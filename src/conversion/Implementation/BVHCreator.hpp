#ifndef BVHCREATOR_HPP
#define BVHCREATOR_HPP

// C++
#include <memory>
// MOAB
#include <moab/Core.hpp>
#include <moab/GeomTopoTool.hpp>
#include <moab/EntityHandle.hpp>
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

        void loadFile(const char* fileName);
        void constructOBBTree();

    private:
        std::shared_ptr<moab::Interface> moabImpl;
        std::shared_ptr<moab::GeomTopoTool> GTT;
        moab::EntityHandle fileSet;
    };
}
#endif //BVHCREATOR_HPP
