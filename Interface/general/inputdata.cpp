// McCAD
#include "inputdata.hpp"
#include "inputdata_impl.hpp"

McCAD::General::InputData::InputData()
    : pImpl{std::make_unique<Impl>()}{
}

McCAD::General::InputData::InputData(
        const InputData& that)
    : pImpl{new Impl{*that.pImpl}}{
}

McCAD::General::InputData::InputData(
        InputData&& that)
    : pImpl{std::move(that.pImpl)}{
}

McCAD::General::InputData::~InputData(){
}

McCAD::General::InputData&
McCAD::General::InputData::operator=(
        const InputData& that){
    this->pImpl.reset(new Impl{*that.pImpl});
    return *this;
}

McCAD::General::InputData&
McCAD::General::InputData::operator=(
        InputData&& that){
    this->pImpl = std::move(that.pImpl);
    return *this;
}

McCAD::General::InputData::Impl*
McCAD::General::InputData::accessImpl() const{
    return pImpl.get();
}

int
McCAD::General::InputData::getSize(){
    return pImpl->size;
}

