#ifndef EDGE_HPP
#define EDGE_HPP

// C++
#include <memory>
// McCAD
#include <Standard.hxx>

namespace McCAD::Decomposition {
    class Edge {
    private:
      class Impl;
      
    public:
      Edge();
      ~Edge();
      
      Impl* accessEImpl() const;

    private:
      std::unique_ptr<Impl> pEImpl;
    };
}
#endif //EDGE_HPP
