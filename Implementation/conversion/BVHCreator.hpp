#ifndef BVHCREATOR_HPP
#define BVHCREATOR_HPP

// C++
#include <memory>
//OCC
#include <TopTools_HSequenceOfShape.hxx>

namespace McCAD::Conversion::Impl {
    class BVHCreator {
    public:
        BVHCreator(const std::shared_ptr<TopTools_HSequenceOfShape> solidsList);
        ~BVHCreator();
    };
}
#endif //BVHCREATOR_HPP
