#ifndef TOOLS_HPP
#define TOOLS_HPP

// C++
#include <memory>
// McCAD

namespace McCAD::Tools {
  class Preprocessor {
  private:
    class Impl;
    
  public:
    Preprocessor();
    ~Preprocessor();

    Impl* accessImpl() const;
    
  private:
    std::unique_ptr<Impl> pImpl;
  };
}

#endif //TOOLS_HPP
