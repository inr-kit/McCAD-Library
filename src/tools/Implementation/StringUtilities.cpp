//C++
#include <sstream>
#include <algorithm>
// McCAD
#include "StringUtilities.hpp"

/** ********************************************************************
* @brief   Splits a line with a specified delimiter.
* @param   line is a string to split.
* @param   delimiter is a character delimiter.
* @return  A vector of strings.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
std::vector<std::string>
McCAD::Tools::splitLine(const std::string& line, char delimiter){
    std::istringstream ss(line);
    std::vector<std::string> lineSplit;
    std::string s;
    while (std::getline(ss, s, delimiter)){
        lineSplit.push_back(s);
    }
    return lineSplit;
}

/** ********************************************************************
* @brief   Converts a string to lower case.
* @param   string is a string to convert to lower case.
* @return  A lower case string.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
std::string
McCAD::Tools::stringToLowerCase(std::string string){
    std::transform(string.begin(), string.end(), string.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return string;
}
