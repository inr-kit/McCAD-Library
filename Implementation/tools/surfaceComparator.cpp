// McCAD
#include "surfaceComparator.hpp"
#include "planeComparator.hpp"

Standard_Boolean
McCAD::Tools::SurfaceComparator::operator()(const TopoDS_Face& firstFace,
                                            const TopoDS_Face& secondFace) const{
    // Compare orientations
    if(firstFace.Orientation() != secondFace.Orientation()){
        //std::cout << "Orientation different" << std::endl;
        return Standard_False;
    }
    TopLoc_Location firstLocation;
    GeomAdaptor_Surface firstAdaptor{
        BRep_Tool::Surface(firstFace, firstLocation)};

    TopLoc_Location secondLocation;
    GeomAdaptor_Surface secondAdaptor{
        BRep_Tool::Surface(secondFace, secondLocation)};
    // Compare surfaces types.
    if(firstAdaptor.GetType() != secondAdaptor.GetType()){
        //std::cout << "Orientation different" << std::endl;
        return Standard_False;
    }
    // Compare locations
    if(firstLocation != secondLocation){
        //std::cout << "Location different" << std::endl;
        return Standard_False;
    }

    // Compare surfaces parameters
    if (firstAdaptor.GetType() == GeomAbs_Plane &&
            firstAdaptor.GetType() == GeomAbs_Plane){
        auto condition = PlaneComparator{}(firstAdaptor, secondAdaptor);
        return condition.value();
    }
    else if (firstAdaptor.GetType() == GeomAbs_Cylinder
             && firstAdaptor.GetType() == GeomAbs_Cylinder)
        return Standard_False; //CylComparator{}(firstAdaptor, secondAdaptor);
    else return Standard_False;
}
