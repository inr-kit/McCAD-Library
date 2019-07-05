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

      std::string surfaceType;
      Standard_Integer surfaceNumber;

    private:
      std::unique_ptr<Impl> pImpl;
    };
}
#endif //SURFACE_HPP
