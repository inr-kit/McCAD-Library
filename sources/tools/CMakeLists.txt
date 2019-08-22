add_library(TOOLS
  tools.cpp
  tools_impl.cpp
  CurveUtilities.cpp
  SurfaceUtilities.cpp
  PlaneComparator.cpp
  PlaneFuser.cpp
  FaceParameters.cpp)

install (TARGETS TOOLS DESTINATION ${PROJECT_BINARY_DIR}/bin)

