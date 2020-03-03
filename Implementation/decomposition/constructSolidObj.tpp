//McCAD
#include "constructSolidObj.hpp"
#include "solid_impl.hpp"
#include "generateFacesList.hpp"

template<typename SolidObjType>
std::shared_ptr<SolidObjType>
McCAD::Decomposition::constructObj(const TopoDS_Shape& shape){
    std::shared_ptr<SolidObjType> solidObj = std::make_shared<SolidObjType>();
    auto& solidImpl = *solidObj->accessSImpl();
    solidImpl.initiate(shape);
    solidImpl.createOBB();
    solidImpl.calcMeshDeflection();
    solidImpl.updateEdgesConvexity();
    solidImpl.facesList = GenerateFacesList{}(*solidObj);
    solidImpl.repairSolid();
    return solidObj;
}
