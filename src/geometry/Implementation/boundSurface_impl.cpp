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
#include "EdgesComparator.hpp"
#include "EdgeType.hpp"
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
#include <GeomAbs_CurveType.hxx>
#include <STEPControl_Writer.hxx>

McCAD::Geometry::BoundSurface::Impl::Impl(BoundSurface* backReference)
  : boundSurface{backReference}{
}

McCAD::Geometry::BoundSurface::Impl::~Impl(){
}

bool
McCAD::Geometry::BoundSurface::Impl::isEqual(const McCAD::Geometry::BoundSurface& that){
    return Tools::SurfaceComparator{}(boundSurface->accessSImpl()->face,
                                      that.accessSImpl()->face);
}

bool
McCAD::Geometry::BoundSurface::Impl::canFuse(const McCAD::Geometry::BoundSurface& that){
    // Check common edges of the two faces.
    for (int i = 0; i < edgesList.size(); ++i){
        for (int j = 0; j < that.accessBSImpl()->edgesList.size(); ++j){
            if(Tools::EdgesComparator{}(edgesList[i]->accessEImpl()->edge,
                                        that.accessBSImpl()->edgesList[j]->accessEImpl()->edge)){
                return true;
            }
        }
    }
    return false;
}

/** ********************************************************************
* @brief    A function that generates a McCAD triangle objects for a given OOCT face.
* @param    meshDeflection is a calculated mesh size.
* @return   true if mesh generation succeeded, otherwise false.
* @date     23/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
bool
McCAD::Geometry::BoundSurface::Impl::generateMesh(const double& meshDeflection){
  TopoDS_Face face = boundSurface->accessSImpl()->face;
  // Generate mesh of the surface.
  try{
      TopLoc_Location location;
      Handle_Poly_Triangulation mesh;
      BRepMesh_IncrementalMesh incMesh(face, meshDeflection, true);
      incMesh.Perform();
      mesh = BRep_Tool::Triangulation(face, location);
      if (!mesh.IsNull()){
          BRepAdaptor_Surface surface(face, true);
          gp_Trsf Transformation = surface.Trsf();
          // Get mesh nodes.
          int numberNodes = mesh->NbNodes();
          TColgp_Array1OfPnt meshNodes(1, numberNodes);
          meshNodes = mesh->Nodes();
          // Get mesh triangles.
          int numberTriangles = mesh->NbTriangles();
          const Poly_Array1OfTriangle& Triangles = mesh->Triangles();
          std::array<int, 3> triangleNodes;
          for (const auto& Triangle : Triangles){
              Triangle.Get(triangleNodes[0], triangleNodes[1], triangleNodes[2]);
              std::array<gp_Pnt, 3> points = {
                  meshNodes(triangleNodes[0]).Transformed(Transformation),
                  meshNodes(triangleNodes[1]).Transformed(Transformation),
                  meshNodes(triangleNodes[2]).Transformed(Transformation)};
              // Generate new face with the retrieved triangle points.
              TopoDS_Wire wire = BRepBuilderAPI_MakePolygon(points[0], points[1],
                      points[2], true);
              TopoDS_Face triangleFace = BRepBuilderAPI_MakeFace(wire, true);
              std::shared_ptr<MeshTriangle> meshTriangle = std::make_shared<MeshTriangle>();
              meshTriangle->accessMTImpl()->initiate(triangleFace);
              meshTriangle->accessMTImpl()->points = points;
              meshTrianglesList.push_back(meshTriangle);
          }
          return true;
      } else return false;
  } catch(...) {
      return false;
  }
}

/** ********************************************************************
* @brief    A function that generates McCAD edge objects for edges of a OOCT face.
* @param    parameterTolerance is a tolerance set in inputConfig file and is used to compare UV parameters.
* @date     23/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
void
McCAD::Geometry::BoundSurface::Impl::generateEdges(const double& parameterTolerance){
    TopoDS_Face face = boundSurface->accessSImpl()->face;
    for (const auto& tempEdge : detail::ShapeView<TopAbs_EDGE>{face}){
        // Ignore degenerated edges.
        if(BRep_Tool::Degenerated(tempEdge)) continue;
        // In the fututre make more specific edge types as with surfaces and solids
        std::shared_ptr<Edge> edge = std::make_shared<Edge>();
        edge->accessEImpl()->initiate(tempEdge);
        // Get type of Edge.
        BRepAdaptor_Curve curveAdaptor(tempEdge);
        edge->setEdgeType(Tools::toTypeName(curveAdaptor.GetType()));
        edge->accessEImpl()->convexity = tempEdge.Convex();
        if (tempEdge.Convex() == Tools::EdgeType{}.concave) {
            boundSurface->accessSImpl()->throughConcaveEdges += 1;
        }
        // Add flag if the edge can be used for assisting splitting surface.
        if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Cylinder) &&
                edge->getEdgeType() == Tools::toTypeName(GeomAbs_Line)){
            std::array<double, 4> edgeUV, surfaceUV;
            BRepTools::UVBounds(face, tempEdge, edgeUV[0], edgeUV[1], edgeUV[2],
                    edgeUV[3]);
            BRepTools::UVBounds(face, surfaceUV[0], surfaceUV[1], surfaceUV[2],
                    surfaceUV[3]);
            if (std::abs(edgeUV[0] - surfaceUV[0]) < parameterTolerance ||
                    std::abs(edgeUV[1] - surfaceUV[1]) < parameterTolerance){
                edge->accessEImpl()->useForSplitSurface = true;
            }
        }
        edgesList.push_back(edge);
    }
}

void
McCAD::Geometry::BoundSurface::Impl::combineEdges(std::vector<std::shared_ptr<Edge>>& aEdgesList){
    if (edgesList.size() == 0){
        // If the current list is empty, append to it the new one.
        for (int i = 0; i < aEdgesList.size(); ++i){
            edgesList.push_back(aEdgesList[i]);
        }
    } else{
        // Compare and add only if different.
        for (int i = 0; i < aEdgesList.size(); ++i){
            for (int j = 0; j < edgesList.size(); ++j){
                if (*(edgesList[j]) == *(aEdgesList[i])){
                    /* //debug
                    STEPControl_Writer writer1;
                    writer1.Transfer(edgesList[j]->accessEImpl()->edge,
                                     STEPControl_StepModelType::STEPControl_AsIs);
                    writer1.Transfer(edgesList[i]->accessEImpl()->edge,
                                     STEPControl_StepModelType::STEPControl_AsIs);
                    int kk = 0;
                    std::string filename = "/home/mharb/opt/McCAD_refactor/examples/bbox/edges";
                    std::string suffix = ".stp";
                    while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
                        ++kk;
                    }
                    filename += std::to_string(kk);
                    filename += suffix;
                    writer1.Write(filename.c_str());
                    */ //debug
                    edgesList.erase(edgesList.begin() + j);
                    --j;
                }
            }
            edgesList.push_back(aEdgesList[i]);
        }
    }
}

bool
McCAD::Geometry::BoundSurface::Impl::generateParmts(double precision,
                                                    double scalingFactor){
    if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Plane)){
        // std::vector<gp_Pln, gp_Pnt, gp_Dir, parameters>
        auto generatedParmts = Tools::FaceParameters{precision, scalingFactor}.genPlSurfParmts(
                    boundSurface->accessSImpl()->face);
        boundSurface->accessSImpl()->plane = std::get<0>(generatedParmts);
        boundSurface->accessSImpl()->location = std::get<1>(generatedParmts);
        boundSurface->accessSImpl()->normal = std::get<2>(generatedParmts);
        boundSurface->accessSImpl()->surfParameters.insert(
                    boundSurface->accessSImpl()->surfParameters.end(),
                    std::get<3>(generatedParmts).begin(),
                    std::get<3>(generatedParmts).end());
    } else if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Cylinder)){
        // std::vector<gp_Cylinder, gp_Pnt, gp_Dir, parameters, radius, sense>
        auto generatedParmts = Tools::FaceParameters{precision, scalingFactor}.genCylSurfParmts(
                    boundSurface->accessSImpl()->face);
        boundSurface->accessSImpl()->cylinder = std::get<0>(generatedParmts);
        boundSurface->accessSImpl()->location = std::get<1>(generatedParmts);
        boundSurface->accessSImpl()->symmetryAxis = std::get<2>(generatedParmts);
        boundSurface->accessSImpl()->surfParameters.insert(
                    boundSurface->accessSImpl()->surfParameters.end(),
                    std::get<3>(generatedParmts).begin(),
                    std::get<3>(generatedParmts).end());
        boundSurface->accessSImpl()->radius = std::get<4>(generatedParmts);
        boundSurface->accessSImpl()->surfSense = std::get<5>(generatedParmts);
    } else if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Torus)){
        // std::tuple<gp_Torus, gp_Pnt, gp_Dir, parameters, minorRadius, majorRadius, sense>
        auto generatedParmts = Tools::FaceParameters{precision, scalingFactor}.genTorSurfParmts(
                    boundSurface->accessSImpl()->face);
        boundSurface->accessSImpl()->torus = std::get<0>(generatedParmts);
        boundSurface->accessSImpl()->location = std::get<1>(generatedParmts);
        boundSurface->accessSImpl()->symmetryAxis = std::get<2>(generatedParmts);
        boundSurface->accessSImpl()->surfParameters.insert(
                    boundSurface->accessSImpl()->surfParameters.end(),
                    std::get<3>(generatedParmts).begin(),
                    std::get<3>(generatedParmts).end());
        boundSurface->accessSImpl()->minorRadius = std::get<4>(generatedParmts);
        boundSurface->accessSImpl()->majorRadius = std::get<5>(generatedParmts);
        boundSurface->accessSImpl()->surfSense = std::get<6>(generatedParmts);
    }
    else return false;
    return true;
}
