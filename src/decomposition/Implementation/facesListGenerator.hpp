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
  public:
      FacesListGenerator() = default;
  private:
      Standard_Real precision{precision};
      using BS = std::shared_ptr<Geometry::BoundSurface>;
      std::vector<BS> facesList;
      std::vector<BS> planesList;
      std::vector<BS> cylindersList;
      std::vector<BS> toriList;
    public:
      template <typename solidObjType>
      std::vector<BS> operator()(solidObjType& solidObj, const Standard_Real& precision,
                                 const Standard_Real& edgeTolerance);
      template <typename solidObjType>
      void addListsToSolidObj(solidObjType& solidObj);
      void mergePlanesList(const Standard_Real& boxDiagonalLength);
      void mergeCylindersList(const Standard_Real& boxDiagonalLength);
      void mergeToriList(const Standard_Real& boxDiagonalLength);
    };
}

#include "facesListGenerator.tpp"

#endif //FACESLISTGENERATOR_HPP
