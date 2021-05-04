#ifndef MCNPWRITER_HPP
#define MCNPWRITER_HPP

// C++
#include <string>
#include <vector>
#include <memory>
#include <iostream>
// McCAD
#include "solid_impl.hpp"
#include "voidCell.hpp"
// OCC

namespace McCAD::Conversion{
    class MCNPWriter {
    public:
        MCNPWriter(const std::string& MCOutputFileName, const int& startCellNum,
                   const int& startSurfNum);
        ~MCNPWriter();
    private:
        using solidsList = std::vector<std::shared_ptr<Geometry::Solid>>;
    public:
        std::string MCOutputFileName;
        int startCellNum, startSurfNum;
        void processSolids(const solidsList& solidObjList);
        void processVoids(const std::shared_ptr<VoidCell>& voidCell);
        void writeHeader(ofstream& outputStream,
                         const McCAD::Conversion::MCNPWriter::solidsList& solidObjList);
        void writeCellCard(ofstream& outputStream,
                           const McCAD::Conversion::MCNPWriter::solidsList& solidObjList);
        void writeSurfCard(ofstream& outputStream,
                           const McCAD::Conversion::MCNPWriter::solidsList& solidObjList);
        void writeDataCard(ofstream& outputStream);
    };
}

#endif // MCNPWRITER_HPP
