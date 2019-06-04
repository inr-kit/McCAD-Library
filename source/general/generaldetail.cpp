// McCAD
#include "generaldetail.hpp"

McCAD::General::InputData::Impl::Impl(
        const std::string& fileName)
    : fileName{fileName}{
}

void
McCAD::General::InputData::Impl::setFileName(
        const std::string& fileName){
    this->fileName = fileName;
};

const std::string&
McCAD::General::InputData::Impl::getFileName() const{
    return fileName;
}


