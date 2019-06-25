// McCAD
#include "tools.hpp"
#include "tools_impl.hpp"

McCAD::Tools::Preprocessor::Impl*
McCAD::Tools::Preprocessor::accessImpl() const{
  return pImpl.get();
}

