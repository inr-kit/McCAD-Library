// McCAD
#include "conversion.hpp"
#include "conversion_impl.hpp"

/** ********************************************************************
* @brief   Constructor for the conversion class.
* @details The constructor passes the inputConfig parameters to the conversion implementation class and creates a unique pointer.
* @param   inputConfig object.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
McCAD::Conversion::Convert::Convert(IO::InputConfig& inputConfig) :
  pCImpl{std::make_unique<Impl>(inputConfig)}{
}

McCAD::Conversion::Convert::~Convert(){
}

/** ********************************************************************
* @brief   This function is a gateway to access the implementation of the conversion algorithm.
* @return  a unique pointer to the conversion implementation, Convert::Impl, class.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
McCAD::Conversion::Convert::Impl*
McCAD::Conversion::Convert::accessCImpl() const{
  return pCImpl.get();
}
