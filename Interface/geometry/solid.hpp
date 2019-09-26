#ifndef SOLID_HPP
#define SOLID_HPP

// C++
#include <memory>
// McCAD
#include <Standard.hxx>
// OCC

namespace McCAD::Geometry{
  class Solid {
  public:
    class Impl;

  public:
    Solid();
    ~Solid();

    Impl* accessSImpl() const;
    
  private:
    std::unique_ptr<Impl> pSImpl;
  };
}
#endif //SOLID_HPP
