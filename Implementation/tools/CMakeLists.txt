add_library(Tools
    tools_impl.cpp
    CurveUtilities.cpp
    SurfaceUtilities.cpp
    PlaneComparator.cpp
    PlaneFuser.cpp
    FaceParameters.cpp
    )

install(FILES
  tools_impl.hpp
  CurveUtilities.hpp
  SurfaceUtilities.hpp
  PlaneComparator.hpp
  PlaneFuser.hpp
  FaceParameters.hpp
  ShapeView.hpp
  DESTINATION ${PROJECT_BINARY_DIR}/includes)
  
add_library(Tools_impl tools_impl.cpp)
add_subdirectory(FloatingPoint)
