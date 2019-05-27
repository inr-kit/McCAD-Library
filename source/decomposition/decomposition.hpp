#ifndef DECOMPOSITION_HPP
#define DECOMPOSITION_HPP

// C++ includes
#include <iostream>

// McCAD includes
#include "solidscollection.hpp"

namespace McCAD { namespace Decomp {

    class Decomposition {
    public:
      Decomposition() = default;
      Decomposition(const Interface::SolidsCollection& raw);

      void run();
      const Interface::SolidsCollection& getDecomposed() const;

    private:
      Interface::SolidsCollection raw;
      Interface::SolidsCollection decomposed;
	
    };

}}

#endif //DECOMPOSITION_HPP
