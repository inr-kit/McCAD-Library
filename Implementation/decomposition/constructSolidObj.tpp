//McCAD
#include "constructSolidObj.hpp"
#include "generateFacesList.hpp"

template<typename solidObjType>
std::shared_ptr<solidObjType>
McCAD::Decomposition::SolidObjConstructor::constructObj(const TopoDS_Shape& shape){
    std::shared_ptr<solidObjType> solidObj = std::make_shared<solidObjType>();
    auto& solidImpl = *solidObj->accessSImpl();
    // Initiate solid.
    solidImpl.initiate(shape);
    // Calculate OBB of the solid.
    solidImpl.createOBB();
    // Calculate mesh deflection of the solid.
    solidImpl.calcMeshDeflection();
    // Update edges convexity of the solid.
    //std::cout << "solidImpl.updateEdgesConvexity();" << std::endl;
    solidImpl.updateEdgesConvexity();
    // Generate the boundary surfaces list of the solid.
    //std::cout << "solidImpl.generateSurfacesList();" << std::endl;
    solidImpl.facesList = GenerateFacesList{}(*solidObj);
    // Repair solid.
    solidImpl.repairSolid();
    return solidObj;
}
