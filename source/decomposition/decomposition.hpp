#ifndef DECOMPOSITION_HPP
#define DECOMPOSITION_HPP

#include "solidscollection.hpp"
#include <iostream>

namespace McCAD { namespace Decomp {

    class Decomposition {
    public:
      Decomposition() = default;
      Decomposition(const Universal::SolidsCollection& raw);

      void run();
      const Universal::SolidsCollection& getDecomposed() const;

    private:
      Universal::SolidsCollection raw;
      Universal::SolidsCollection decomposed;
	
    };

}}

#endif //DECOMPOSITION_HPP
