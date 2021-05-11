#ifndef MCNPWRITER_HPP
#define MCNPWRITER_HPP

// C++
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <optional>
// McCAD
#include "solid_impl.hpp"
#include "voidCell.hpp"
#include "inputconfig.hpp"
// OCC
#include <Standard.hxx>

namespace McCAD::Conversion{
    class MCNPWriter {
    public:
        MCNPWriter(const IO::InputConfig& inputConfig);
        ~MCNPWriter();
    private:
        using solidsList = std::vector<std::shared_ptr<Geometry::Solid>>;
        using surfacesMap = std::map<Standard_Integer, std::shared_ptr<Geometry::BoundSurface>>;
        using finalMap = std::map<Standard_Integer, std::string>;
        using cellsMap = std::map<Standard_Integer, std::vector<std::shared_ptr<Geometry::Solid>>>;
        using solidsMap = std::map<Standard_Integer, std::shared_ptr<Geometry::Solid>>;
        using voidsMap = std::map<std::tuple<Standard_Integer, Standard_Integer>,
                                  std::shared_ptr<VoidCell>>;
    public:
        Standard_Integer maxLineWidth;
        Standard_Real precision;
        std::string MCOutputFileName, volumesFileName;
        Standard_Integer startCellNum, startSurfNum;
        surfacesMap uniquePlanes, uniqueCylinders, uniqueSpheres;
        finalMap uniqueSurfaces;
        cellsMap componentsMap;
        solidsMap solidObjMap;
        voidsMap voidCellsMap;
        Standard_Boolean voidGeneration, BVHVoid;
        Standard_Real radius;

        void operator()(const solidsList& solidObjList,
                        const std::shared_ptr<VoidCell>& voidCell);
        void processSolids(const solidsList& solidObjList);
        void processVoids(const std::shared_ptr<VoidCell>& voidCell);
        void addUniqueSurfNumbers(const solidsList& solidObjList);
        std::optional<Standard_Integer> findDuplicate(
                const std::shared_ptr<Geometry::BoundSurface>& surface,
                surfacesMap& uniqueMap);
        void createComponentMap(const solidsList& solidObjList);
        void addDaughterVoids(const std::shared_ptr<VoidCell>& voidCell);
        void createVoidMap(const std::shared_ptr<VoidCell>& voidCell);
        std::string adjustLineWidth(const std::string& mainExpr,
                                    const std::string& bodyExpr);
        void writeHeader(ofstream& outputStream);
        void writeCellCard(ofstream& outputStream, ofstream& volumeStream);
        void writeVoidCard(ofstream& outputStream);
        void writeSurfCard(ofstream& outputStream);
        void writeDataCard(ofstream& outputStream);
    };
}

#endif // MCNPWRITER_HPP
