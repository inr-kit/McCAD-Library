//McCAD
#include "constructSolidObj.hpp"
#include "generateFacesList.hpp"

template<typename solidObjType>
std::shared_ptr<solidObjType>
McCAD::Decomposition::SolidObjConstructor::constructObj(const TopoDS_Shape& shape){
    std::shared_ptr<solidObjType> solidObj = std::make_shared<solidObjType>();
    auto& solidImpl = *solidObj->accessSImpl();
    solidImpl.initiate(shape);
    solidImpl.createOBB();
    solidImpl.calcMeshDeflection();
    solidImpl.updateEdgesConvexity();
    // Generate a list of the boundary surfaces of the solid.
    solidImpl.facesList = GenerateFacesList{}(*solidObj);
    solidImpl.repairSolid();
    return solidObj;
}
