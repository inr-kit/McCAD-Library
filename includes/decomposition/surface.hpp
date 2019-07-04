#ifndef SURFACE_HPP
#define SURFACE_HPP

// C++
#include <memory>
#include <string>
// McCAD
#include <Standard.hxx>

namespace McCAD::Decomposition {
    class Surface {
    private:
      class Impl;
      
    public:
      Surface();
      ~Surface();
      
      Impl* accessImpl() const;

      void setSurfaceType(const std::string& surfacetype);
      std::string getSurfaceType();
      void setSurfaceNumber(const Standard_Integer& surfaceNumber);
      Standard_Integer getSurfaceNUmber();

    private:
      std::unique_ptr<Impl> pImpl;
    };
}
#endif //SURFACE_HPP
