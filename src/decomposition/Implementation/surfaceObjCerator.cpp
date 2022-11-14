//McCAD
#include "surfaceObjCerator.hpp"
#include "boundSurfacePlane_impl.hpp"
#include "boundSurfaceCyl_impl.hpp"
#include "boundSurfaceTor_impl.hpp"
#include "boundSurfaceCone_impl.hpp"
#include "SurfaceUtilities.hpp"
// OCC
#include <BRepAdaptor_Surface.hxx>
#include <GeomAdaptor_Surface.hxx>

/** ********************************************************************
* @brief    This operator creates surface objects corresponding to the OCCT face type.
* @param    face is a OCCT face.
* @param    boxDiagonalLength is the calculated diagonal length of the solid OBB.
* @param    edgeTolerance is set on inputConfig file.
* @param    mode is defaulted to 0.
* @return   A McCAD bound surface object.
* @date     31/12/2020
* @modified 23/08/2022
* @author   Moataz Harb
* **********************************************************************/
std::shared_ptr<McCAD::Geometry::BoundSurface>
McCAD::Decomposition::SurfaceObjCreator::operator()(const TopoDS_Face& face,
                                                    const double& boxDiagonalLength,
                                                    const double& edgeTolerance,
                                                    int mode){
    if (mode == int(0)){
        BRepAdaptor_Surface surface(face, true);
        GeomAdaptor_Surface AdaptorSurface = surface.Surface();
        if (AdaptorSurface.GetType() == GeomAbs_Plane){
            std::shared_ptr<Geometry::BoundSurfacePlane> boundSurfacePlane =
                    std::make_shared<Geometry::BoundSurfacePlane>();
            boundSurfacePlane->setSurfaceType(Tools::toTypeName(GeomAbs_Plane));
            boundSurfacePlane->accessSImpl()->initiate(face);
            boundSurfacePlane->accessBSPImpl()->generateExtendedPlane(boxDiagonalLength,
                                                                      edgeTolerance);
            return boundSurfacePlane;
        } else if (AdaptorSurface.GetType() == GeomAbs_Cylinder){
            std::shared_ptr<Geometry::BoundSurfaceCyl> boundSurfaceCyl =
                    std::make_shared<Geometry::BoundSurfaceCyl>();
            boundSurfaceCyl->setSurfaceType(Tools::toTypeName(GeomAbs_Cylinder));
            boundSurfaceCyl->accessSImpl()->initiate(face);
            boundSurfaceCyl->accessBSCImpl()->generateExtendedCyl(boxDiagonalLength,
                                                                  edgeTolerance);
            return boundSurfaceCyl;
        } else if (AdaptorSurface.GetType() == GeomAbs_Torus){
            std::shared_ptr<Geometry::BoundSurfaceTor> boundSurfaceTor =
                    std::make_shared<Geometry::BoundSurfaceTor>();
            boundSurfaceTor->setSurfaceType(Tools::toTypeName(GeomAbs_Torus));
            boundSurfaceTor->accessSImpl()->initiate(face);
            //boundSurfaceTor->accessBSTImpl()->generateExtendedTor(boxDiagonalLength, edgeTolerance);
            return boundSurfaceTor;
        } else if (AdaptorSurface.GetType() == GeomAbs_Cone) {
            std::shared_ptr<Geometry::BoundSurfaceCone> boundSurfaceCone =
                std::make_shared<Geometry::BoundSurfaceCone>();
            boundSurfaceCone->setSurfaceType(Tools::toTypeName(GeomAbs_Cone));
            boundSurfaceCone->accessSImpl()->initiate(face);
            boundSurfaceCone->accessBSConImpl()->generateExtendedCone(boxDiagonalLength,
                edgeTolerance);
            return boundSurfaceCone;
        }
    }
    return nullptr;
}
