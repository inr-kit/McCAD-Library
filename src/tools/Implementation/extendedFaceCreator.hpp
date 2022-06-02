#ifndef EXTENDEDFACECREATOR_HPP
#define EXTENDEDFACECREATOR_HPP

// OCC
#include <Standard.hxx>
#include <TopoDS_Face.hxx>

namespace McCAD::Tools{
  class ExtendedFaceCreator{
  public:
      ExtendedFaceCreator();
      ExtendedFaceCreator(const Standard_Real& edgeTolerance);
      ~ExtendedFaceCreator();
  private:
      Standard_Real edgeTolerance{1.0e-7};
  public:
      TopoDS_Face generateExtendedPlane(const TopoDS_Face& face,
                                        const Standard_Real& boxDiagonalLength);
    };
}

#endif //EXTENDEDFACECREATOR_HPP
