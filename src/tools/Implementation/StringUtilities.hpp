#ifndef STRINGUTILITIES_HPP
#define STRINGUTILITIES_HPP

//C++
#include <vector>

namespace McCAD::Tools{
        std::vector<std::string> splitLine(const std::string& line, char delimiter);
        std::string stringToLowerCase(std::string string);
}

#endif
