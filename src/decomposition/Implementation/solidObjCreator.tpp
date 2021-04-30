//McCAD
#include "solidObjCreator.hpp"
#include "solid_impl.hpp"
#include "generateFacesList.hpp"

template<typename solidObjType>
std::shared_ptr<solidObjType>
McCAD::Decomposition::SolidObjCreator::createObj(const TopoDS_Shape& shape){
    std::shared_ptr<solidObjType> solidObj = std::make_shared<solidObjType>();
    auto& solidImpl = *solidObj->accessSImpl();
    solidImpl.initiate(shape);
    solidImpl.createOBB();
    solidImpl.calcMeshDeflection();
    solidImpl.updateEdgesConvexity();
    solidImpl.facesList = FacesListGenerator{}(*solidObj);
    solidImpl.repairSolid();
    return solidObj;
}
