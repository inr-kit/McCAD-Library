#ifndef DECOMPOSITION_HPP
#define DECOMPOSITION_HPP

// C++
#include <memory>
// McCAD
#include "inputdata.hpp"

namespace McCAD::Decomposition {
    class Decompose {
    private:
      class Impl;
      
    public:
      Decompose(const McCAD::General::InputData& inputData);
      ~Decompose();
      
      Impl* accessImpl() const;

    private:
      std::unique_ptr<Impl> pImpl;
    };
}
#endif //DECOMPOSITION_HPP
