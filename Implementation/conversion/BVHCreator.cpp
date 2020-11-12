// McCAD
#include "BVHCreator.hpp"
// MOAB
//#include <moab/ReaderIface.hpp>
//#include "moab/CartVect.hpp"
//#include "moab/Range.hpp"
//#include "moab/GeomUtil.hpp"
//#include "moab/FileOptions.hpp"
//#include "moab/GeomTopoTool.hpp"
//#include "moab/GeomQueryTool.hpp"

McCAD::Conversion::BVHCreator::BVHCreator(const std::string& inputFileName){
    moab::EntityHandle fileHandle, meshHandle;
    moabImpl = std::make_shared<moab::Core>();
    //moabImpl->create_meshset(moab::MESHSET_SET, meshHandle);
    //moabImpl->load_file(inputFileName.c_str(), fileHandle);
}

McCAD::Conversion::BVHCreator::~BVHCreator(){}
