// McCAD
#include "BVHCreator.hpp"
// MOAB
//#include <moab/ReadCGM.hpp>
#include <moab/ReaderIface.hpp>
#include "moab/CartVect.hpp"
#include "moab/Range.hpp"
#include "moab/GeomUtil.hpp"
#include "moab/FileOptions.hpp"
#include "moab/GeomTopoTool.hpp"
#include "moab/GeomQueryTool.hpp"

McCAD::Conversion::BVHCreator::BVHCreator(const IO::InputConfig& inputConfig,
        const std::shared_ptr<TopTools_HSequenceOfShape> inputSolidsList)
    : moabImpl{std::make_shared<moab::Core>()}{
    char options[120] = {0};
    moab::ErrorCode rval;
    moab::EntityHandle file_set;
    rval = moabImpl->create_meshset(moab::MESHSET_SET, file_set);
    if (moab::MB_SUCCESS != rval){
        std::cout << "failed to create meshset!" << std::endl;
        return;
    } else{
        rval = moabImpl->load_file(inputConfig.conversionFileName.c_str(), &file_set,
                                   options, NULL, 0, 0);
    }

    // find all geometry sets
    std::shared_ptr<moab::GeomTopoTool> GTT =
            std::make_shared<moab::GeomTopoTool> (moabImpl.get(), false);
    rval = GTT->find_geomsets();
    if (moab::MB_SUCCESS != rval){
        std::cout << "GeomTopoTool could not find the geometry sets" << std::endl;
    }

    //moab::EntityHandle fileHandle, meshHandle;
    //moabImpl->create_meshset(moab::MESHSET_SET, meshHandle);
    //moabImpl->load_file(inputFileName.c_str(), fileHandle);
}

McCAD::Conversion::BVHCreator::~BVHCreator(){}
