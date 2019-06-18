// McCAD
#include "tools.hpp"
#include "tools_impl.hpp"

McCAD::Tools::Preprocessor::Preprocessor()
  : pImpl{std::make_unique<Impl>()}{
}

McCAD::Tools::Preprocessor::~Preprocessor(){
}

McCAD::Tools::Preprocessor::Impl*
McCAD::Tools::Preprocessor::accessImpl(){
  return pImpl.get();
}

