add_subdirectory(FloatingPoint)

add_library(TOOLS_IMPL OBJECT
    tools_impl.cpp
    CurveUtilities.cpp
    EdgesComparator.cpp
    SurfaceUtilities.cpp
    surfaceComparator.cpp
    planeComparator.cpp
    surfacesFuser.cpp
    FaceParameters.cpp
    senseEvaluator.cpp
    heirarchyFlatter.cpp)

install(FILES
  tools_impl.hpp
  CurveUtilities.hpp
  EdgesComparator.hpp
  SurfaceUtilities.hpp
  surfaceComparator.hpp
  planeComparator.hpp
  surfacesFuser.hpp
  FaceParameters.hpp
  ShapeView.hpp
  SolidType.hpp
  senseEvaluator.hpp
  heirarchyFlatter.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/implementation/tools)

target_link_libraries(TOOLS_IMPL
    BOOST
    OCC)
