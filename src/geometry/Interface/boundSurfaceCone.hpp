#ifndef BOUNDSURFACECONE_HPP
#define BOUNDSURFACECONE_HPP

// C++
#include <memory>
// McCAD
#include "boundSurface.hpp"

namespace McCAD::Geometry{
    class BoundSurfaceCone : public BoundSurface {
        private:
            class Impl;

        public:
            BoundSurfaceCone();
            ~BoundSurfaceCone();
    
            Impl* accessBSConImpl() const;

        private:
            std::unique_ptr<Impl> pBSConImpl;
  };
}
#endif //BOUNDSURFACECONE_HPP
