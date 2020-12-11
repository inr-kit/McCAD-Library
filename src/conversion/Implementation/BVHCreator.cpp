// McCAD
#include "BVHCreator.hpp"
// MOAB
#include <MBEntityType.h>
#include <MBTagConventions.hpp>
#include <moab/ReaderWriterSet.hpp>
#include <moab/ReaderIface.hpp>
//#include "moab/CartVect.hpp"
//#include "moab/Range.hpp"
#include "moab/GeomUtil.hpp"
#include "moab/FileOptions.hpp"
#include "moab/GeomQueryTool.hpp"

McCAD::Conversion::BVHCreator::BVHCreator(const IO::InputConfig& inputConfig,
        const std::shared_ptr<TopTools_HSequenceOfShape> inputSolidsList)
    : moabImpl{std::make_shared<moab::Core>()}{
    loadFile(inputConfig.conversionFileName.c_str());
    constructOBBTree();
}

McCAD::Conversion::BVHCreator::~BVHCreator(){}

void
McCAD::Conversion::BVHCreator::loadFile(const char* fileName){
    char options[120] = {0};
    moabImpl->create_meshset(moab::MESHSET_SET, fileSet);
    moabImpl->load_file(fileName, &fileSet, options);
}

void
McCAD::Conversion::BVHCreator::constructOBBTree(){
    GTT = std::make_shared<moab::GeomTopoTool> (moabImpl.get(), true);
    GTT->setup_implicit_complement();
    GTT->delete_all_obb_trees();
    std::cout << "vols: " << GTT->num_ents_of_dim(3) <<
                 "\nsurfs: " << GTT->num_ents_of_dim(2) << std::endl;
    GTT->construct_obb_trees();
}
