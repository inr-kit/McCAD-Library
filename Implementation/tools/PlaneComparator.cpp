// McCAD
#include "PlaneComparator.hpp"

McCAD::Tools::PlaneComparator::PlaneComparator(Standard_Real parameterTolerance,
					       Standard_Real angularTolerance,
					       Standard_Real distanceTolerance)
  : parameterTolerance{parameterTolerance},
    angularTolerance{angularTolerance},
    distanceTolerance{distanceTolerance}{
}

bool
McCAD::Tools::PlaneComparator::operator()(const TopoDS_Face& firstFace,
					  const TopoDS_Face& secondFace) const{
    // Compare orientations
    if(firstFace.Orientation() != secondFace.Orientation())
        return Standard_False;

    // Compare locations
    TopLoc_Location firstLocation;
    GeomAdaptor_Surface firstAdaptor{
        BRep_Tool::Surface(firstFace, firstLocation)};

    TopLoc_Location secondLocation;
    GeomAdaptor_Surface secondAdaptor{
        BRep_Tool::Surface(secondFace, secondLocation)};

    if(firstLocation != secondLocation)
        return Standard_False;

    try
      {
        auto firstPlane = firstAdaptor.Plane();             // also checks if surface type is GeomAbs_Plane
        auto secondPlane = secondAdaptor.Plane();           // also checks if surface type is GeomAbs_Plane

	//std::cout << "distance: " << firstPlane.Distance(secondPlane) << std::endl;
        secondPlane.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);
        firstPlane.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);
	//std::cout << "distance: " << firstPlane.Distance(secondPlane) << std::endl;

        // Compare plane parameters
        return equivalentPlaneParameters(firstPlane, secondPlane);
      }
    catch(const Standard_NoSuchObject&)
      {
        // GeomAdaptor_Surface::Plane() failed: one surface is not a plane
        return Standard_False;
      }
}

std::array<Standard_Real, 4>
McCAD::Tools::PlaneComparator::planeParameters(const gp_Pln& plane) const{
    std::array<Standard_Real, 4> planeParameters;
    plane.Coefficients(
                planeParameters[0],
                planeParameters[1],
                planeParameters[2],
                planeParameters[3]);
    for(auto& parameter : planeParameters){
        if(parameter <= parameterTolerance)
            parameter = 0.0;
    }

    return planeParameters;
}

bool
McCAD::Tools::PlaneComparator::equivalentPlaneParameters(const gp_Pln& first,
							 const gp_Pln& second) const{

    try
      {
        auto firstPlaneParameters = planeParameters(first);

        gp_Dir firstPlaneDirection{
            firstPlaneParameters[0],
            firstPlaneParameters[1],
            firstPlaneParameters[2]
        };

        auto secondPlaneParameters = planeParameters(second);

        gp_Dir secondPlaneDirection{
            secondPlaneParameters[0],
            secondPlaneParameters[1],
            secondPlaneParameters[2]
        };

	//std::cout << "firstPlane: " << firstPlaneParameters[3] << std::endl;
	//std::cout << (firstPlaneDirection.IsEqual(secondPlaneDirection, angularTolerance) && std::abs(firstPlaneParameters[3] - secondPlaneParameters[3]) <= distanceTolerance) << std::endl;
	//std::cout << "secondPlane: " << secondPlaneParameters[3] << std::endl;
	//std::cout << (firstPlaneDirection.IsOpposite(secondPlaneDirection, angularTolerance) && std::abs(firstPlaneParameters[3] + secondPlaneParameters[3]) <= distanceTolerance) << std::endl;

        return (firstPlaneDirection.IsEqual(secondPlaneDirection, angularTolerance)
                && std::abs(firstPlaneParameters[3] - secondPlaneParameters[3])
                    <= distanceTolerance)
                ||
	  
               (firstPlaneDirection.IsOpposite(secondPlaneDirection, angularTolerance)
                && std::abs(firstPlaneParameters[3] + secondPlaneParameters[3])
                    <= distanceTolerance);
      }
    catch(const Standard_ConstructionError&)
      {
        // Plane parameters cannot form a normed direction vector;
        // not distinguishable from zero vector
        return Standard_False;
      }
}
