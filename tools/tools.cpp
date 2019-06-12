// McCAD
#include "tools.hpp"
#include "tools_impl.hpp"

McCAD::Tools::Preprocessor::Preprocessor()
  : pImpl{std::make_unique<Impl>()}{
}

McCAD::Tools::Preprocessor::~Preprocessor(){
}

const McCAD::Tools::Preprocessor::Impl*
McCAD::Tools::Preprocessor::accessImpl() const{
  return pImpl.get();
}

