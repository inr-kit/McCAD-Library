#ifndef MESHTRIANGLE_HPP
#define MESHTRIANGLE_HPP

// C++
#include <memory>
#include <string>

namespace McCAD::Geometry {
  class MeshTriangle {
  private:
    class Impl;

  public:
    MeshTriangle();
    ~MeshTriangle();
      
    Impl* accessMTImpl() const;

  private:
    std::unique_ptr<Impl> pMTImpl;
  };
}

#endif //MESHTRIANGLE_HPP
