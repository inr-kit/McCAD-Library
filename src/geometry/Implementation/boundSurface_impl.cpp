//C++
#include <filesystem>
#include <tuple>
#include <array>
// McCAD
#include "boundSurface_impl.hpp"
#include "surfaceComparator.hpp"
#include "ShapeView.hpp"
#include "CurveUtilities.hpp"
#include "SurfaceUtilities.hpp"
#include "faceParameters.hpp"
//OCC
#include <TopoDS_Face.hxx>
#include <TopLoc_Location.hxx>
#include <Poly_Triangulation.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <gp_Trsf.hxx>
#include <BRep_Tool.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TopoDS_Wire.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepTools.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <STEPControl_Writer.hxx>

McCAD::Geometry::BoundSurface::Impl::Impl(BoundSurface* backReference)
  : boundSurface{backReference}{
}

McCAD::Geometry::BoundSurface::Impl::~Impl(){
}

Standard_Boolean
McCAD::Geometry::BoundSurface::Impl::isEqual(const McCAD::Geometry::BoundSurface& that){
    return Tools::SurfaceComparator{}(boundSurface->accessSImpl()->face,
                                      that.accessSImpl()->face);
}

Standard_Boolean
McCAD::Geometry::BoundSurface::Impl::canFuse(const McCAD::Geometry::BoundSurface& that){
    // Check common edges of the two faces.
    for (Standard_Integer i = 0; i <= edgesList.size() - 2; ++i){
        for (Standard_Integer j = i+1; j <= that.accessBSImpl()->edgesList.size() - 1;
             ++j){
            if (*edgesList[i] == *that.accessBSImpl()->edgesList[j]){
                /* //debug
                STEPControl_Writer writer3;
                writer3.Transfer(edgesList[i]->accessEImpl()->edge,
                                 STEPControl_StepModelType::STEPControl_AsIs);
                writer3.Transfer(that.accessBSImpl()->edgesList[j]->accessEImpl()->edge,
                                 STEPControl_StepModelType::STEPControl_AsIs);
                Standard_Integer kk = 0;
                std::string filename = "/home/mharb/Documents/McCAD_refactor/examples/bbox/edge";
                std::string suffix = ".stp";
                while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
                    ++kk;
                }
                filename += std::to_string(kk);
                filename += suffix;
                writer3.Write(filename.c_str());
                */ //debug
                return Standard_True;
            }
        }
    }
    return Standard_False;
}

Standard_Boolean
McCAD::Geometry::BoundSurface::Impl::generateMesh(const Standard_Real& meshDeflection){
  TopoDS_Face face = boundSurface->accessSImpl()->face;
  // Generate mesh of the surface.
  try{
      TopLoc_Location location;
      Handle_Poly_Triangulation mesh;
      BRepMesh_IncrementalMesh incMesh(face, meshDeflection, Standard_True);
      incMesh.Perform();
      mesh = BRep_Tool::Triangulation(face, location);
      if (!mesh.IsNull()){
          BRepAdaptor_Surface surface(face, Standard_True);
          gp_Trsf Transformation = surface.Trsf();
          // Get mesh nodes.
          Standard_Integer numberNodes = mesh->NbNodes();
          TColgp_Array1OfPnt meshNodes(1, numberNodes);
          meshNodes = mesh->Nodes();
          // Get mesh triangles.
          Standard_Integer numberTriangles = mesh->NbTriangles();
          const Poly_Array1OfTriangle& Triangles = mesh->Triangles();
          std::array<Standard_Integer, 3> triangleNodes;
          for (const auto& Triangle : Triangles){
              Triangle.Get(triangleNodes[0], triangleNodes[1], triangleNodes[2]);
              //std::cout << triangleNodes[0] << std::endl;
              //std::cout	<< triangleNodes[1] << std::endl;
              //std::cout	<< triangleNodes[2] << std::endl;
              //std::cout << "=====" << std::endl;
              std::array<gp_Pnt, 3> points = {
                  meshNodes(triangleNodes[0]).Transformed(Transformation),
                  meshNodes(triangleNodes[1]).Transformed(Transformation),
                  meshNodes(triangleNodes[2]).Transformed(Transformation)};
              // Generate new face with the retrieved triangle points.
              TopoDS_Wire wire = BRepBuilderAPI_MakePolygon(points[0], points[1],
                      points[2], Standard_True);
              TopoDS_Face triangleFace = BRepBuilderAPI_MakeFace(wire, Standard_True);
              std::shared_ptr<MeshTriangle> meshTriangle = std::make_shared<MeshTriangle>();
              meshTriangle->accessMTImpl()->initiate(triangleFace);
              meshTriangle->accessMTImpl()->points = points;
              meshTrianglesList.push_back(meshTriangle);
          }
          return Standard_True;
      } else return Standard_False;
  } catch(...) {
      return Standard_False;
  }
}

void
McCAD::Geometry::BoundSurface::Impl::generateEdges(Standard_Real uvTolerance){
    TopoDS_Face face = boundSurface->accessSImpl()->face;
    for (const auto& tempEdge : detail::ShapeView<TopAbs_EDGE>{face}){
        // more specific edge types as with surfaces and solids
        std::shared_ptr<Edge> edge = std::make_shared<Edge>();
        edge->accessEImpl()->initiate(tempEdge);
        // Get type of Edge.
        BRepAdaptor_Curve curveAdaptor(tempEdge);
        edge->setEdgeType(Tools::toTypeName(curveAdaptor.GetType()));
        edge->accessEImpl()->convexity = tempEdge.Convex();
        if (tempEdge.Convex() == Standard_Integer(0)){
            boundSurface->accessSImpl()->throughConcaveEdges += 1;
        }
        // Add flag if the edge can be used for assisting splitting surface.
        if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Cylinder) &&
                edge->getEdgeType() == Tools::toTypeName(GeomAbs_Line)){
            std::array<Standard_Real, 4> edgeUV, surfaceUV;
            BRepTools::UVBounds(face, tempEdge, edgeUV[0], edgeUV[1], edgeUV[2],
                    edgeUV[3]);
            BRepTools::UVBounds(face, surfaceUV[0], surfaceUV[1], surfaceUV[2],
                    surfaceUV[3]);
            if (std::abs(edgeUV[0] - surfaceUV[0]) < uvTolerance ||
                    std::abs(edgeUV[1] - surfaceUV[1]) < uvTolerance){
                edge->accessEImpl()->useForSplitSurface = Standard_True;
            }
        }
        edgesList.push_back(edge);
    }
}

void
McCAD::Geometry::BoundSurface::Impl::combineEdges(std::vector<std::shared_ptr<Edge>>& aEdgesList){
    //std::cout << "combineEdges" << std::endl;
    if (edgesList.size() == 0){
        // If the current list is empty, append to it the new one.
        for (Standard_Integer i = 0; i <= aEdgesList.size() - 1; ++i){
            //std::cout << "combineEdges, add all" << std::endl;
            edgesList.push_back(aEdgesList[i]);
        }
    } else{
        // Compare and add only if different.
        //std::cout << "combineEdges, add different" << std::endl;
        for (Standard_Integer i = 0; i <= aEdgesList.size() - 1; ++i){
            Standard_Integer sameEdge = 0;
            for (Standard_Integer j = 0; j <= edgesList.size() - 1; ++j){
                //std::cout << "i: " << i << ", j: " << j << std::endl;
                if (*(edgesList[j]) == *(aEdgesList[i])){
                    /* //debug
                    STEPControl_Writer writer1;
                    writer1.Transfer(edgesList[j]->accessEImpl()->edge,
                                     STEPControl_StepModelType::STEPControl_AsIs);
                    writer1.Transfer(edgesList[i]->accessEImpl()->edge,
                                     STEPControl_StepModelType::STEPControl_AsIs);
                    Standard_Integer kk = 0;
                    std::string filename = "/home/mharb/opt/McCAD_refactor/examples/bbox/edges";
                    std::string suffix = ".stp";
                    while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
                        ++kk;
                    }
                    filename += std::to_string(kk);
                    filename += suffix;
                    writer1.Write(filename.c_str());
                    */ //debug
                    //std::cout << "combineEdges, equal, erase" << std::endl;
                    edgesList.erase(edgesList.begin() + j);
                    ++sameEdge;
                    --j;
                    //std::cout << edgesList.size() << ", j: " << j << std::endl;
                }
            }
            if (sameEdge == 0){
                edgesList.push_back(aEdgesList[i]);
            }
        }
    }
}

Standard_Boolean
McCAD::Geometry::BoundSurface::Impl::generateParmts(){
    if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Plane)){
        // std::vector<gp_Pln, gp_Pnt, gp_Dir, parameters>
        auto generatedParmts = Tools::FaceParameters{1.0e-7}.genPlSurfParmts(
                    boundSurface->accessSImpl()->face);
        boundSurface->accessSImpl()->plane = std::get<0>(generatedParmts);
        boundSurface->accessSImpl()->location = std::get<1>(generatedParmts);
        boundSurface->accessSImpl()->normal = std::get<2>(generatedParmts);
        boundSurface->accessSImpl()->surfParameters = std::get<3>(generatedParmts);
                //Tools::genPlSurfParmts(boundSurface->accessSImpl()->face);
    } else if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Cylinder))
        Tools::FaceParameters{1.0e-7}.genCylSurfParmts(boundSurface->accessSImpl()->face);
    else if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Torus))
        Tools::FaceParameters{1.0e-7}.genTorSurfParmts(boundSurface->accessSImpl()->face);
    else return Standard_False;
    return Standard_True;
}
