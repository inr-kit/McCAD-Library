#ifndef SPLITSURFACES_HPP
#define SPLITSURFACES_HPP

// C++
#include <memory>
#include <string>
// McCAD
#include <Standard.hxx>

namespace McCAD::Decomposition {
    class SplitSurfaces {
    private:
      class Impl;
      
    public:
      SplitSurfaces();
      ~SplitSurfaces();
      
      Impl* accessSSImpl() const;

    private:
      std::unique_ptr<Impl> pSSImpl;
    };
}
#endif //SPLITSURFACES_HPP
