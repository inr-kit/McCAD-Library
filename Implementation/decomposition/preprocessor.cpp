// McCAD
#include "preprocessor.hpp"

McCAD::Decomposition::Preprocessor::Preprocessor(){
}

McCAD::Decomposition::Preprocessor::~Preprocessor(){
}

std::unique_ptr<McCAD::Geometry::Solid>
McCAD::Decomposition::Preprocessor::perform(const TopoDS_Shape& shape){
    TopoDS_Solid solid = TopoDS::Solid(shape);
    if (!checkBndSurfaces(solid)){
        if (determineSolidType(solid) == "Pl"){
            //std::variant<McCAD::Geometry::Solid, std::monostate>
             //       solidVariant(constructObj<McCAD::Geometry::Solid>(solid));
            return constructObj<McCAD::Geometry::Solid>(shape);
        } else{
            std::cout << "Processing of solids with non-planar surfaces is not"
                         "yet supported!. Solid will be added to rejected solids"
                         "file" << std::endl;
        }
    }
}

Standard_Boolean
McCAD::Decomposition::Preprocessor::checkBndSurfaces(const TopoDS_Solid& solid){
  for(const auto& face : ShapeView<TopAbs_FACE>{solid})
    {
      GeomAdaptor_Surface surfAdaptor(BRep_Tool::Surface(face));
      if(surfAdaptor.GetType() == GeomAbs_Torus)
        {
          std::cout << "    -- The current verion doesn't support processing of "
                       "Tori. Ignoring solid!" << std::endl;
          return Standard_True;
        }
      else if (surfAdaptor.GetType() == GeomAbs_BSplineSurface)
        {
          std::cout << "    -- The current verion doesn't support processing of "
                       "splines. Ignoring solid!" << std::endl;
          return Standard_True;
        }
    }
}

std::string
McCAD::Decomposition::Preprocessor::determineSolidType(const TopoDS_Solid& solid){
    Standard_Boolean cylSolid, sphSolid, plSolid, mxdSolid;
    for(const auto& face : ShapeView<TopAbs_FACE>{solid}){
        GeomAdaptor_Surface surfAdaptor(BRep_Tool::Surface(face));
        switch (surfAdaptor.GetType()){
        case  GeomAbs_Plane:
            plSolid = Standard_True;
            break;
        case GeomAbs_Cylinder:
            cylSolid = Standard_True;
            break;
        case GeomAbs_Sphere:
            sphSolid = Standard_True;
            break;
        default:
            mxdSolid = Standard_True;
        }
    }
    // Determine solid type based on surfaces types.
    // This is to be modified as was done in SurfaceUtilities.
    if (mxdSolid || (cylSolid && sphSolid)) return "Mxd";
    else if (sphSolid) return "Sph";
    else if (cylSolid) return "Cyl";
    else return "Pl";
}

template<typename objType>
std::unique_ptr<objType>
McCAD::Decomposition::Preprocessor::constructObj(const TopoDS_Shape& shape){
    std::unique_ptr<objType> solidObj = std::make_unique<objType>();
    auto& solidImpl = *solidObj->accessSImpl();
    // Initiate solid.
    solidImpl.initiate(shape);
    // Calculate OBB of the solid.
    solidImpl.createOBB();
    // Calculate mesh deflection of the solid.
    solidImpl.calcMeshDeflection();
    // Update edges convexity of the solid.
    solidImpl.updateEdgesConvexity();
    // Generate the boundary surfaces list of the solid.
    solidImpl.generateSurfacesList();
    // Judge which surfaces are decompose surfaces from the generated list.
    return solidObj;
}
