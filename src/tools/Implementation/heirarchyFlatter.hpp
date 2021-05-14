#ifndef HEIRARCHYFLATTER_HPP
#define HEIRARCHYFLATTER_HPP

// C++
#include <memory>
#include <vector>
#include <tuple>
// OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Shape.hxx>
#include <TCollection_ExtendedString.hxx>

namespace McCAD::Tools{
  class HeirarchyFlatter{
  public:
      HeirarchyFlatter();
      ~HeirarchyFlatter();

  private:
      using shape_Name = std::vector<std::tuple<TopoDS_Shape, TCollection_ExtendedString>>;
      using shape_Name_ID = std::vector<std::tuple<TopoDS_Shape, TCollection_ExtendedString,
                                                   Standard_Integer>>;
      using output_pair = std::pair<std::unique_ptr<TopTools_HSequenceOfShape>,
                                    std::unique_ptr<TopTools_HSequenceOfShape>>;
      using output_MapPair = std::pair<shape_Name_ID, shape_Name_ID>;

  public:
      std::unique_ptr<TopTools_HSequenceOfShape> splitInputSolidsList;
      std::unique_ptr<TopTools_HSequenceOfShape> rejectedInputSolidsList;
      shape_Name_ID splitInputSolidsMap, rejectedInputSolidsMap;

      void process(const TopoDS_Shape& shape);
      output_MapPair operator()(const shape_Name& inputSolidsMap);
      output_pair operator()(const TopoDS_Shape& shape);
      output_pair operator()(const std::shared_ptr<TopTools_HSequenceOfShape>& inputShapesList);
  };
}

#endif //HEIRARCHYFLATTER_HPP
