#ifndef DECOMPOSITION_HPP
#define DECOMPOSITION_HPP

// C++
#include <memory>
// McCAD

namespace McCAD::Decomposition {
    class Decompose {
    private:
      class Impl;
      
    public:
      Decompose(const auto& inputDataObj);
      ~Decompose();
      void perform(McCAD::General::InputData& inputData);
      Impl* accessImpl() const;

    private:
      std::unique_ptr<Impl> pImpl;
    };
}
#endif //DECOMPOSITION_HPP
