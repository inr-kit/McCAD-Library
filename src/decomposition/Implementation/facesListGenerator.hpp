#ifndef FACESLISTGENERATOR_HPP
#define FACESLISTGENERATOR_HPP

//C++
#include <memory>
#include <vector>
//McCAD
#include "boundSurface_impl.hpp"
//OCC
#include <Standard.hxx>

namespace McCAD::Decomposition{
  class FacesListGenerator{
  private:
      using BS = std::shared_ptr<Geometry::BoundSurface>;
      std::vector<BS> facesList;
      std::vector<BS> planesList;
      std::vector<BS> cylindersList;
      std::vector<BS> toriList;
    public:
      template <typename solidObjType>
      std::vector<BS> operator()(solidObjType& solidObj);
      template <typename solidObjType>
      void addListsToSolidObj(solidObjType& solidObj);
      void mergePlanesList(Standard_Real& boxDiagonalLength);
      void mergeCylindersList(Standard_Real& boxDiagonalLength);
      void mergeToriList(Standard_Real& boxDiagonalLength);
    };
}

//McCAD
#include "facesListGenerator.tpp"

#endif //FACESLISTGENERATOR_HPP
