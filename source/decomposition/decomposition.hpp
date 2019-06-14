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
      void predecompose();
      void perform();

    private:
      class Impl;
      std::unique_ptr<Impl> pImpl;

    public:
      // Allow access to Impl class from other classes.
      const McCAD::Decomposition::Decompose::Impl* accessImpl() const;
      
    };
}
#endif //DECOMPOSITION_HPP
