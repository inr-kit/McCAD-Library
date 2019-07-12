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
      
      Impl* accessSImpl() const;

    private:
      std::unique_ptr<Impl> pSImpl;
    };
}
#endif //SURFACE_HPP
