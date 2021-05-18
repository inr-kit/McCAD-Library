#ifndef HEIRARCHYFLATTER_HPP
#define HEIRARCHYFLATTER_HPP

// C++
#include <memory>
// OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <TopoDS_Shape.hxx>
#include <TCollection_ExtendedString.hxx>

namespace McCAD::Tools{
  class HeirarchyFlatter{
  public:
      HeirarchyFlatter();
      ~HeirarchyFlatter();
  private:
      using output_pair = std::pair<std::unique_ptr<TopTools_HSequenceOfShape>,
                                    std::unique_ptr<TopTools_HSequenceOfShape>>;
  public:
      std::unique_ptr<TopTools_HSequenceOfShape> splitInputSolidsList;
      std::unique_ptr<TopTools_HSequenceOfShape> rejectedInputSolidsList;

      void process(const TopoDS_Shape& shape);
      output_pair operator()(const TopoDS_Shape& shape);
      output_pair operator()(const std::shared_ptr<TopTools_HSequenceOfShape>& inputShapesList);
  };
}

#endif //HEIRARCHYFLATTER_HPP
