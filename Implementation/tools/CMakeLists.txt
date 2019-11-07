add_library(TOOLS_IMPL OBJECT
    tools_impl.cpp
    CurveUtilities.cpp
    SurfaceUtilities.cpp
    surfaceComparator.cpp
    planeComparator.cpp
    PlaneFuser.cpp
    FaceParameters.cpp)

install(FILES
  tools_impl.hpp
  CurveUtilities.hpp
  SurfaceUtilities.hpp
  surfaceComparator.hpp
  planeComparator.hpp
  PlaneFuser.hpp
  FaceParameters.hpp
  ShapeView.hpp
  SolidType.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes/implementation/tools)

add_subdirectory(FloatingPoint)

target_link_libraries(TOOLS_IMPL
  OCC)
