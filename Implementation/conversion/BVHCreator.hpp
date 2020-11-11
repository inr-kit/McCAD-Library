#ifndef BVHCREATOR_HPP
#define BVHCREATOR_HPP

// C++
#include <memory>
//OCC
#include <TopTools_HSequenceOfShape.hxx>
//MOAB
#include <moab/Core.hpp>

namespace McCAD::Conversion::Impl {
    class BVHCreator {
    public:
        BVHCreator(const std::shared_ptr<TopTools_HSequenceOfShape> solidsList);
        ~BVHCreator();
    };
}
#endif //BVHCREATOR_HPP
