//McCAD
#include "createSolidObj.hpp"
#include "solid_impl.hpp"
#include "generateFacesList.hpp"

template<typename solidObjType>
std::shared_ptr<solidObjType>
McCAD::Decomposition::SolidObjCreator::createObj(const TopoDS_Shape& shape){
    //std::cout << "Preprocessor::constructObj" << std::endl;
    std::shared_ptr<solidObjType> solidObj = std::make_shared<solidObjType>();
    auto& solidImpl = *solidObj->accessSImpl();
    solidImpl.initiate(shape);
    solidImpl.createOBB();
    solidImpl.calcMeshDeflection();
    solidImpl.updateEdgesConvexity();
    // Generate the boundary surfaces list of the solid.
    //std::cout << "solidImpl.generateSurfacesList();" << std::endl;
    solidImpl.facesList = FacesListGenerator{}(*solidObj);
    // Repair solid.
    solidImpl.repairSolid();
    return solidObj;
}
