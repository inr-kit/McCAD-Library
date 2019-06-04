// McCAD
#include "general.hpp"
#include "generaldetail.hpp"

McCAD::General::InputData::InputData()
    : pImpl{std::make_unique<Impl>()}{
}

McCAD::General::InputData::InputData(
        const std::string& fileName)
    : pImpl{std::make_unique<Impl>(fileName)}{
}

McCAD::General::InputData::~InputData(){
}

void
McCAD::General::InputData::setFileName(
        const std::string& fileName){
    pImpl->setFileName(fileName);
}

const std::string&
McCAD::General::InputData::getFileName() const{
    return pImpl->getFileName();
}




