#ifndef FACESLISTGENERATOR_HPP
#define FACESLISTGENERATOR_HPP

//C++
#include <memory>
#include <vector>
//McCAD
#include "boundSurface_impl.hpp"

namespace McCAD::Decomposition{
  class FacesListGenerator{
  public:
      FacesListGenerator() = default;
  private:
      using BS = std::shared_ptr<Geometry::BoundSurface>;
      std::vector<BS> facesList;
      std::vector<BS> planesList;
      std::vector<BS> cylindersList;
      std::vector<BS> toriList;
      std::vector<BS> conesList;
    public:
      template <typename solidObjType>
      std::vector<BS> operator()(solidObjType& solidObj,
                                 const double& precision,
                                 const double& edgeTolerance,
                                 const double& faceTolerance,
                                 const double& angularTolerance,
                                 const double& distanceTolerance,
                                 const double& parameterTolerance);
      template <typename solidObjType>
      void addListsToSolidObj(solidObjType& solidObj,
                              const double& precision,
                              const double& edgeTolerance,
                              const double& angularTolerance,
                              const double& distanceTolerance);
      void mergePlanesList(const double& boxDiagonalLength,
                           const double& precision,
                           const double& edgeTolerance,
                           const double& angularTolerance,
                           const double& distanceTolerance);
      void mergeCylindersList(const double& boxDiagonalLength,
                              const double& precision,
                              const double& edgeTolerance,
                              const double& angularTolerance,
                              const double& distanceTolerance);
      void mergeToriList(const double& boxDiagonalLength,
                         const double& precision,
                         const double& edgeTolerance,
                         const double& angularTolerance,
                         const double& distanceTolerance);
      void mergeConesList(const double& boxDiagonalLength,
                          const double& precision,
                          const double& edgeTolerance,
                          const double& angularTolerance,
                          const double& distanceTolerance);
    };
}

#include "facesListGenerator.tpp"

#endif //FACESLISTGENERATOR_HPP
