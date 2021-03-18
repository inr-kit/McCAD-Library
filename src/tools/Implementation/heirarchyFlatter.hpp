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
      using zVector = std::vector<std::tuple<TopoDS_Shape, TCollection_ExtendedString>>;
      using output_pair = std::pair<std::unique_ptr<TopTools_HSequenceOfShape>,
                                    std::unique_ptr<TopTools_HSequenceOfShape>>;
      using output_zpair = std::pair<zVector, zVector>;

  public:
      std::unique_ptr<TopTools_HSequenceOfShape> splitInputSolidsList;
      std::unique_ptr<TopTools_HSequenceOfShape> rejectedInputSolidsList;
      zVector zsplitInputSolidsList;
      zVector zrejectedInputSolidsList;

      output_zpair operator()(const zVector& inputSolidsList);
      output_pair flattenSolidHierarchy(const std::shared_ptr<TopTools_HSequenceOfShape>& inputSolidsList);
  };
}

#endif //HEIRARCHYFLATTER_HPP
