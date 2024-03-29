#ifndef TOOLS_HPP
#define TOOLS_HPP

// C++
#include <memory>

namespace McCAD::Tools {
  class Preprocessor {
  private:
    class Impl;
    
  public:
    Preprocessor();
    Preprocessor(const double& precision, const double& faceTolerance);
    ~Preprocessor();

    Impl* accessImpl() const;
    
  private:
    std::unique_ptr<Impl> pImpl;
  };
}

#endif //TOOLS_HPP
