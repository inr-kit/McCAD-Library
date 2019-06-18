#ifndef DECOMPOSITION_HPP
#define DECOMPOSITION_HPP

// C++
#include <memory>
// McCAD

namespace McCAD::Decomposition {
    class Decompose {
    public:
      Decompose();
      ~Decompose();
      void perform(McCAD::General::InputData& inputData);

    private:
      class Impl;
      std::unique_ptr<Impl> pImpl;

    public:
      // Allow access to Impl class from other classes.
      McCAD::Decomposition::Decompose::Impl* accessImpl();
      
    };
}
#endif //DECOMPOSITION_HPP
