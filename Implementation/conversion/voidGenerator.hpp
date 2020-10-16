#ifndef VOIDGENERATOR_HPP
#define VOIDGENERATOR_HPP

// C++
#include <memory>
//OCC
#include <TopTools_HSequenceOfShape.hxx>

namespace McCAD::Conversion::Impl {
    class VoidGenerator {
    public:
        VoidGenerator(const std::shared_ptr<
                      TopTools_HSequenceOfShape> solidsList);
        ~VoidGenerator();
    };
}
#endif //VOIDGENERATOR_HPP
