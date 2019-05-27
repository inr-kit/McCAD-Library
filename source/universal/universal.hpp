#ifndef UNIVERSAL_HPP
#define UNIVERSAL_HPP
#include <string>
#include <locale>

// Define a universal namespace within McCAD. This namespace should host all classes that
// doesn't belong to the other namespaces (Decomp, Void, materials, conversion, etc.). 
namespace McCAD { namespace Universal {

    class SetFileName {
    public:
      SetFileName() = default;
      char Capitalize (const std::string& name);
    };

}}

#endif //UNIVERSAL_HPP
