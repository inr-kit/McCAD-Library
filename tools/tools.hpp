#ifndef TOOLS_HPP
#define TOOLS_HPP

// C++
#include <memory>
// McCAD

namespace McCAD::Tools {
    class Preprocessor {
    public:
      Preprocessor();
      ~Preprocessor();

    private:
      class Impl;
      std::unique_ptr<Impl> pImpl;

    public:
      // Allow access to Impl class from other classes.
      const McCAD::Tools::Preprocessor::Impl* accessImpl() const;
      
    };
}
#endif //TOOLS_HPP
