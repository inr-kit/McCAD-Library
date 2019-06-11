#ifndef DECOMPOSITION_HPP
#define DECOMPOSITION_HPP

// C++
// McCAD

namespace McCAD::Decomposition {
    class Decompose {
    public:
      Decompose();
      ~Decompose();
      void perform();
      int solidNumber = 0;
      
    private:
      class Impl;
      std::unique_ptr<Impl> pImpl;
    };
}
#endif //DECOMPOSITION_HPP
