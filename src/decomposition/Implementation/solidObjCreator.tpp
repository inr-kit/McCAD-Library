//McCAD
#include "solidObjCreator.hpp"
#include "solid_impl.hpp"
#include "generateFacesList.hpp"

template<typename solidObjType>
std::shared_ptr<solidObjType>
McCAD::Decomposition::SolidObjCreator::createObj(const TopoDS_Shape& shape,
                                                 const Standard_Real& scalingFactor,
                                                 const Standard_Real& angularTolerance){
    std::shared_ptr<solidObjType> solidObj = std::make_shared<solidObjType>();
    solidObj->accessSImpl()->initiate(shape);
    solidObj->accessSImpl()->createOBB();
    solidObj->accessSImpl()->calcMeshDeflection(scalingFactor);
    solidObj->accessSImpl()->updateEdgesConvexity(angularTolerance);
    solidObj->accessSImpl()->facesList = FacesListGenerator{}(*solidObj);
    solidObj->accessSImpl()->repairSolid();
    solidObj->accessSImpl()->calcVolume();
    return solidObj;
}
