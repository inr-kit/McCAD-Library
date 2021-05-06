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
// OCC

namespace McCAD::Conversion{
    class MCNPWriter {
    public:
        MCNPWriter(const std::string& MCOutputFileName, const Standard_Integer& startCellNum,
                   const Standard_Integer& startSurfNum, const Standard_Real& precision);
        ~MCNPWriter();
    private:
        using solidsList = std::vector<std::shared_ptr<Geometry::Solid>>;
        using surfacesMap = std::map<Standard_Integer, std::shared_ptr<Geometry::BoundSurface>>;
        using finalMap = std::map<Standard_Integer, std::string>;
        using cellsMap = std::map<Standard_Integer, std::vector<std::shared_ptr<Geometry::Solid>>>;
    public:
        Standard_Real precision;
        std::string MCOutputFileName;
        int startCellNum, startSurfNum;
        surfacesMap uniquePlanes, uniqueCylinders, uniqueSpheres;
        finalMap uniqueSurfaces;
        cellsMap componentsMap;

        void operator()(const solidsList& solidObjList);
        void operator()(const solidsList& solidObjList,
                        const std::shared_ptr<VoidCell>& voidCell);
        void processSolids(const solidsList& solidObjList);
        void processVoids(const std::shared_ptr<VoidCell>& voidCell);
        Standard_Integer addUniqueSurfNumbers(const solidsList& solidObjList);
        std::optional<Standard_Integer> findDuplicate(
                const std::shared_ptr<Geometry::BoundSurface>& surface,
                surfacesMap& uniqueMap);
        void createComponentMap(const solidsList& solidObjList);
        void writeHeader(ofstream& outputStream, const Standard_Integer& numCells,
                         const Standard_Integer& totalSurfNumber);
        void writeCellCard(ofstream& outputStream);
        void writeSurfCard(ofstream& outputStream);
        void writeDataCard(ofstream& outputStream);
    };
}

#endif // MCNPWRITER_HPP
