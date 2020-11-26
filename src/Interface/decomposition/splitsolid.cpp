// McCAD
#include "splitsolid.hpp"
#include "splitsolid_impl.hpp"

McCAD::Decomposition::SplitSolid::SplitSolid()
  : pSSImpl{std::make_unique<Impl>()}{
}

McCAD::Decomposition::SplitSolid::~SplitSolid(){
}

McCAD::Decomposition::SplitSolid::Impl*
McCAD::Decomposition::SplitSolid::accessSSImpl() const{
  return pSSImpl.get();
}
