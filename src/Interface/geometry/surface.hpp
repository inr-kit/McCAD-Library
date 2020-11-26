#ifndef SURFACE_HPP
#define SURFACE_HPP

// C++
#include <memory>
#include <string>
// McCAD
#include <Standard.hxx>

namespace McCAD::Geometry{
    class Surface{
    private:
      class Impl;
      
    public:
      Surface();
      ~Surface();
      
      Impl* accessSImpl() const;
      
      void setSurfaceType(const std::string& surfaceType);
      std::string getSurfaceType() const;

    private:
      std::unique_ptr<Impl> pSImpl;
    };
}
#endif //SURFACE_HPP
