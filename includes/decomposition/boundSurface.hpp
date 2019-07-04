#ifndef BOUNDSURFACE_HPP
#define BOUNDSURFACE_HPP

// C++
#include <memory>
// McCAD
#include "surface.hpp"

namespace McCAD::Decomposition {
  class BoundSurface : public Surface {
    private:
      class Impl;
      
    public:
      BoundSurface();
      ~BoundSurface();
      
      Impl* accessImpl() const;

    private:
      std::unique_ptr<Impl> pImpl;
    };
}
#endif //BOUNDSURFACE_HPP
