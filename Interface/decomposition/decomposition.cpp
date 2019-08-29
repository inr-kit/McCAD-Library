// McCAD
#include "decomposition.hpp"
#include "decomposition_impl.hpp"

McCAD::Decomposition::Decompose::Decompose(const General::InputData& inputData) :
  pDImpl{std::make_unique<Impl>(inputData)}{
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
  for (Standard_Integer i = 1; i <= pDImpl->resultSolidsList->Length(); ++i)
    {
      outputData.accessImpl()->inputSolidsList->Append(pDImpl->resultSolidsList->Value(i));
    }
  return outputData;
}

McCAD::General::InputData
McCAD::Decomposition::Decompose::getRejectedsubSolids(){
  General::InputData outputData;
  for (Standard_Integer i = 1; i <= pDImpl->rejectedsubSolidsList->Length(); ++i)
    {
      outputData.accessImpl()->inputSolidsList->Append(pDImpl->rejectedsubSolidsList->Value(i));
    }
  return outputData;
}

