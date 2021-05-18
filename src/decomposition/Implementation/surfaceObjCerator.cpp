//McCAD
#include "surfaceObjCerator.hpp"
#include "boundSurfacePlane_impl.hpp"
#include "boundSurfaceCyl_impl.hpp"
#include "boundSurfaceTor_impl.hpp"
#include "SurfaceUtilities.hpp"
// OCC
#include <BRepAdaptor_Surface.hxx>
#include <GeomAdaptor_Surface.hxx>

std::shared_ptr<McCAD::Geometry::BoundSurface>
McCAD::Decomposition::SurfaceObjCreator::operator()(const TopoDS_Face& face,
                                                    Standard_Real& boxDiagonalLength,
                                                    Standard_Integer mode){
    if (mode == Standard_Integer(0)){
        //std::cout << "generateSurface" << std::endl;
        BRepAdaptor_Surface surface(face, Standard_True);
        GeomAdaptor_Surface AdaptorSurface = surface.Surface();
        if (AdaptorSurface.GetType() == GeomAbs_Plane){
            //std::cout << "Plane surface" << std::endl;
            std::shared_ptr<Geometry::BoundSurfacePlane> boundSurfacePlane =
                    std::make_shared<Geometry::BoundSurfacePlane>();
            boundSurfacePlane->setSurfaceType(Tools::toTypeName(GeomAbs_Plane));
            boundSurfacePlane->accessSImpl()->initiate(face);
            boundSurfacePlane->accessBSPImpl()->generateExtendedPlane(boxDiagonalLength);
            return boundSurfacePlane;
        } else if (AdaptorSurface.GetType() == GeomAbs_Cylinder){
            //std::cout << "Cylinder surface" << std::endl;
            std::shared_ptr<Geometry::BoundSurfaceCyl> boundSurfaceCyl =
                    std::make_shared<Geometry::BoundSurfaceCyl>();
            boundSurfaceCyl->setSurfaceType(Tools::toTypeName(GeomAbs_Cylinder));
            boundSurfaceCyl->accessSImpl()->initiate(face);
            boundSurfaceCyl->accessBSCImpl()->generateExtendedCyl(boxDiagonalLength);
            return boundSurfaceCyl;
        } else if (AdaptorSurface.GetType() == GeomAbs_Torus){
            //std::cout << "Torus surface" << std::endl;
            std::shared_ptr<Geometry::BoundSurfaceTor> boundSurfaceTor =
                    std::make_shared<Geometry::BoundSurfaceTor>();
            boundSurfaceTor->setSurfaceType(Tools::toTypeName(GeomAbs_Torus));
            boundSurfaceTor->accessSImpl()->initiate(face);
            //boundSurfaceTor->accessBSTImpl()->generateExtendedTor(boxDiagonalLength);
            return boundSurfaceTor;
        }
    }
    return nullptr;
}
