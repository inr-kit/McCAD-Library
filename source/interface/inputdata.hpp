#ifndef INPUTDATA_HPP
#define INPUTDATA_HPP
#include <string>

/*
Define a namespace "Interface" to host all classes that interface between
user input\geometry kernel and decomposition\void\etc.
*/
namespace McCAD { namespace Interface {
    class InputData {
    public:
      InputData() = default;
      /* reserved for the declaration of filename depending on the geometry kernel */
      std::string filename;
      std::string Input_filename;
      std::string SetFileName (decltype(filename)& filename);
    };

    class SolidsCollection {
    public:
    private:
    };

}}

#endif //INPUTDATA_HPP
