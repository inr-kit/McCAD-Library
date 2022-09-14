// McCAD
#include "decomposition.hpp"
#include "decomposition_impl.hpp"

McCAD::Decomposition::Decompose::Decompose(const General::InputData& inputData,
                                           const IO::InputConfig& inputConfig) :
  pDImpl{std::make_unique<Impl>(inputData, inputConfig)}{
}

McCAD::Decomposition::Decompose::~Decompose(){
}

/** ********************************************************************
* @brief    A function that returns a pointer the the decomposition implementation.
* @return   unique pointer.
* @date     31/12/2020
* @author   Moataz Harb
* **********************************************************************/
McCAD::Decomposition::Decompose::Impl*
McCAD::Decomposition::Decompose::accessDImpl() const{
  return pDImpl.get();
}

/** ********************************************************************
* @brief    A function to retrieve the resultant subsolids from the decomposer.
* @return   McCAD data object.
* @date     31/12/2020
* @author   Moataz Harb
* **********************************************************************/
McCAD::General::InputData
McCAD::Decomposition::Decompose::getResultSolids(){
  General::InputData outputData;
  outputData.accessImpl()->outputShapesMap = pDImpl->successDecomposition;
  return outputData;
}

/** ********************************************************************
* @brief    A function to retrieve the rejected solids from the decomposer.
* @return   McCAD data object.
* @date     31/12/2020
* @author   Moataz Harb
* **********************************************************************/
McCAD::General::InputData
McCAD::Decomposition::Decompose::getRejectedSolids(){
  General::InputData outputData;
  outputData.accessImpl()->outputShapesMap = pDImpl->rejectDecomposition;
  return outputData;
}
