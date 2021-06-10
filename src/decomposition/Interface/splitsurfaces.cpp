// McCAD
#include "splitsurfaces.hpp"
#include "splitsurfaces_impl.hpp"

McCAD::Decomposition::SplitSurfaces::SplitSurfaces()
  : pSSImpl{std::make_unique<Impl>()}{
}

McCAD::Decomposition::SplitSurfaces::~SplitSurfaces(){
}

McCAD::Decomposition::SplitSurfaces::Impl*
McCAD::Decomposition::SplitSurfaces::accessSSImpl() const{
  return pSSImpl.get();
}
