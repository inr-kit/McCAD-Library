// McCAD
#include "decomposition.hpp"
#include "decomposition_impl.hpp"

McCAD::Decomposition::Decompose::Decompose(const General::InputData& inputData,
                                           const IO::InputConfig& inputConfig) :
  pDImpl{std::make_unique<Impl>(inputData, inputConfig)}{
}

McCAD::Decomposition::Decompose::~Decompose(){
}

McCAD::Decomposition::Decompose::Impl*
McCAD::Decomposition::Decompose::accessDImpl() const{
  return pDImpl.get();
}

McCAD::General::InputData
McCAD::Decomposition::Decompose::getResultSolids(){
  General::InputData outputData;
  outputData.accessImpl()->outputShapesMap = pDImpl->successDecomposition;
  return outputData;
}

McCAD::General::InputData
McCAD::Decomposition::Decompose::getRejectedSolids(){
  General::InputData outputData;
  outputData.accessImpl()->outputShapesMap = pDImpl->rejectDecomposition;
  return outputData;
}
