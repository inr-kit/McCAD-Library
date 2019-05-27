#include "universal.hpp"

char McCAD::Universal::SetFileName::Capitalize(const std::string& name) {
  std::locale loc;
  return std::toupper(name[0], loc);
}
