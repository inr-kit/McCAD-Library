add_library(TOOLS
    tools_impl.cpp
    CurveUtilities.cpp
    SurfaceUtilities.cpp
    PlaneComparator.cpp
    PlaneFuser.cpp
    FaceParameters.cpp)

install(FILES
  tools_impl.hpp
  CurveUtilities.hpp
  SurfaceUtilities.hpp
  PlaneComparator.hpp
  PlaneFuser.hpp
  FaceParameters.hpp
  ShapeView.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)

add_subdirectory(FloatingPoint)
