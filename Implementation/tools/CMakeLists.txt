add_library(TOOLS_IMPL OBJECT
    tools_impl.cpp
    CurveUtilities.cpp
    SurfaceUtilities.cpp
    surfaceComparator.cpp
    planeComparator.cpp
    surfacesFuser.cpp
    FaceParameters.cpp
    senseEvaluator.cpp)

install(FILES
  tools_impl.hpp
  CurveUtilities.hpp
  SurfaceUtilities.hpp
  surfaceComparator.hpp
  planeComparator.hpp
  surfacesFuser.hpp
  FaceParameters.hpp
  ShapeView.hpp
  solidType.hpp
  senseEvaluator.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/implementation/tools)

add_subdirectory(FloatingPoint)

target_link_libraries(TOOLS_IMPL
  OCC)
