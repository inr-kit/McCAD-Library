#ifndef GENERAL_HPP
#define GENERAL_HPP

// C++
#include <string>
// McCAD


// Define a universal namespace within McCAD. This namespace should host
// all classes that doesn't belong to the other namespaces (Decomp, Void,
// materials, etc.).
namespace McCAD::General {
    class InputData {
    public:
        InputData();
        ~InputData();
        InputData(const std::string& fileName);

        void setFileName(const std::string& fileName);
        const std::string& getFileName() const;

    private:
        class Impl;
        std::unique_ptr<Impl> pImpl;

    };
}

#endif //GENERAL_HPP
