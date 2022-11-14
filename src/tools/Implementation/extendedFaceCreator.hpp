#ifndef EXTENDEDFACECREATOR_HPP
#define EXTENDEDFACECREATOR_HPP

// OCC
#include <TopoDS_Face.hxx>

namespace McCAD::Tools{
  class ExtendedFaceCreator{
  public:
      ExtendedFaceCreator();
      ExtendedFaceCreator(const double& edgeTolerance);
      ~ExtendedFaceCreator();
  private:
      double edgeTolerance{1.0e-7};
  public:
      TopoDS_Face generateExtendedPlane(const TopoDS_Face& face,
                                        const double& boxDiagonalLength);
      TopoDS_Face generateExtendedCone(const TopoDS_Face& face,
                                       const double& boxDiagonalLength);
    };
}

#endif //EXTENDEDFACECREATOR_HPP
