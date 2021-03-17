#ifndef HEIRARCHYFLATTER_HPP
#define HEIRARCHYFLATTER_HPP

// C++
#include <memory>
// OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Compound.hxx>

namespace McCAD::Tools{
  class HeirarchyFlatter{
  public:
      HeirarchyFlatter();
      ~HeirarchyFlatter();

      std::unique_ptr<TopTools_HSequenceOfShape> splitInputSolidsList;
      std::unique_ptr<TopTools_HSequenceOfShape> rejectedInputSolidsList;
      std::pair<std::unique_ptr<TopTools_HSequenceOfShape>,
      std::unique_ptr<TopTools_HSequenceOfShape>> flattenSolidHierarchy(
              const std::shared_ptr<TopTools_HSequenceOfShape>& inputSolidsList);
  };
}

#endif //HEIRARCHYFLATTER_HPP
