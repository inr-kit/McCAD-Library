#include "inputdata.hpp"

/* Function that returns the input file name. The file name should be 
provided by the user as part of the input to run McCAD.
*/
std::string McCAD::Interface::InputData::SetFileName (decltype(filename)& filename){
  Input_filename = std::string(filename);
  return Input_filename;
}
