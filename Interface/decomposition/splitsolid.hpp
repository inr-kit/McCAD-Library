#ifndef SPLITSOLID_HPP
#define SPLITSOLID_HPP

// C++
#include <memory>
#include <string>
// McCAD
#include <Standard.hxx>

namespace McCAD::Decomposition {
    class SplitSolid {
    private:
      class Impl;
      
    public:
      SplitSolid();
      ~SplitSolid();
      
      Impl* accessSSImpl() const;

    private:
      std::unique_ptr<Impl> pSSImpl;
    };
}
#endif //SPLITSOLID_HPP
