#ifndef EDGE_HPP
#define EDGE_HPP

// C++
#include <memory>
#include <string>

namespace McCAD::Geometry{
  class Edge{
  private:
    class Impl;

  public:
    Edge();
    ~Edge();
    bool operator == (const Edge& that);
      
    Impl* accessEImpl() const;

    void setEdgeType(const std::string& edgeType);
    std::string getEdgeType();

  private:
    std::unique_ptr<Impl> pEImpl;
  };
}
#endif //EDGE_HPP
