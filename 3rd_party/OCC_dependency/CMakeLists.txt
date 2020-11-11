if(Use_Linux_OS)
  find_package(OpenCASCADE 7.3.0 EXACT REQUIRED)
  if(OpenCASCADE_FOUND)
    message(STATUS "OpenCASCADE ${OpenCASCADE_VERSION} found")
    set (OCC_TOOLKITS ${OpenCASCADE_LIBRARIES})
  else(OpenCASCADE_FOUND)
    set(OCC_TOOLKITS TKernel TKMath TKG2d TKG3d TKGeomBase TKBRep TKGeomAlgo
      TKTopAlgo TKPrim TKBO TKShHealing TKBool TKHLR TKFillet TKOffset TKFeat
      TKMesh TKXMesh TKService TKV3d TKOpenGl TKMeshVS TKCDF TKLCAF TKCAF TKBinL
      TKXmlL TKBin TKXml TKStdL TKStd TKTObj TKBinTObj TKXmlTObj TKVCAF TKXSBase
      TKSTEPBase TKSTEPAttr TKSTEP209 TKSTEP TKIGES TKXCAF TKXDEIGES TKXDESTEP
      TKSTL TKVRML TKXmlXCAF TKBinXCAF TKDraw TKTopTest TKViewerTest TKXSDRAW
      TKDCAF TKXDEDRAW TKTObjDRAW TKQADraw)
    if(DEFINED ENV{CASROOT})
      if(NOT DEFINED OpenCASCADE_INCLUDE_DIR)
        set(OpenCASCADE_INCLUDE_DIR "$ENV{CASROOT}/include/opencascade")
      endif(NOT DEFINED OpenCASCADE_INCLUDE_DIR)
      if(NOT DEFINED OpenCASCADE_LIBRARY_DIR)
        set(OpenCASCADE_LIBRARY_DIR "$ENV{CASROOT}/lib")
      endif(NOT DEFINED OpenCASCADE_LIBRARY_DIR)
    else(DEFINED ENV{CASROOT})
      if(NOT DEFINED OpenCASCADE_INCLUDE_DIR OR NOT DEFINED OpenCASCADE_LIB_DIR)
        message(WARNING "To specify paths of OpenCascade files, you may either \n 
                         define the CASROOT environment variable, or set both \n 
                         OCC_INCLUDE_DIR and OCC_LIBRARY_DIR variables.")
      endif(NOT DEFINED OpenCASCADE_INCLUDE_DIR OR NOT DEFINED OpenCASCADE_LIB_DIR)
    endif(DEFINED ENV{CASROOT})
    if(DEFINED OpenCASCADE_INCLUDE_DIR)
      message(STATUS "OpenCASCADE search path for include files: OpenCASCADE_INCLUDE_DIR=${OpenCASCADE_INCLUDE_DIR}")
      include_directories(${OpenCASCADE_INCLUDE_DIR})
    endif(DEFINED OpenCASCADE_INCLUDE_DIR)
  endif(OpenCASCADE_FOUND)

  if(DEFINED OpenCASCADE_INCLUDE_DIR AND DEFINED OpenCASCADE_LIBRARY_DIR)
      add_library(OCC INTERFACE)
      install (TARGETS OCC DESTINATION ${PROJECT_BINARY_DIR}/bin)
      target_link_directories(OCC INTERFACE ${OpenCASCADE_LIBRARY_DIR})
      target_link_libraries(OCC INTERFACE ${OCC_TOOLKITS})
      target_include_directories(OCC INTERFACE ${OpenCASCADE_INCLUDE_DIR})
      message(STATUS "OCC include_dir ${OpenCASCADE_INCLUDE_DIR}")
  endif(DEFINED OpenCASCADE_INCLUDE_DIR AND DEFINED OpenCASCADE_LIBRARY_DIR)

## =========================================================

else(Use_Linux_OS)
  message("WINDOWS")
  add_library(OCC INTERFACE)

  set(OCC_LIB_DIR
    "C:/OpenCASCADE/OpenCASCADE-7.3.0-vc14-64/opencascade-7.3.0/win64/vc14/lib")
  set(OCC_INC_DIR
    "C:/OpenCASCADE/OpenCASCADE-7.3.0-vc14-64/opencascade-7.3.0/inc")

  target_link_libraries(OCC
    INTERFACE
    "${OCC_LIB_DIR}/TKBin.lib"
    "${OCC_LIB_DIR}/TKBinL.lib"
    "${OCC_LIB_DIR}/TKBinTObj.lib"
    "${OCC_LIB_DIR}/TKBinXCAF.lib"
    "${OCC_LIB_DIR}/TKBO.lib"
    "${OCC_LIB_DIR}/TKBool.lib"
    "${OCC_LIB_DIR}/TKBRep.lib"
    "${OCC_LIB_DIR}/TKCAF.lib"
    "${OCC_LIB_DIR}/TKCDF.lib"
    "${OCC_LIB_DIR}/TKLCAF.lib"
    "${OCC_LIB_DIR}/TKSTL.lib"
    "${OCC_LIB_DIR}/TKXMesh.lib"
    "${OCC_LIB_DIR}/TKernel.lib"
    "${OCC_LIB_DIR}/TKMath.lib"
    "${OCC_LIB_DIR}/TKService.lib"
    "${OCC_LIB_DIR}/TKTObj.lib"
    "${OCC_LIB_DIR}/TKXml.lib"
    "${OCC_LIB_DIR}/TKFeat.lib"
    "${OCC_LIB_DIR}/TKMesh.lib"
    "${OCC_LIB_DIR}/TKTopAlgo.lib"
    "${OCC_LIB_DIR}/TKXmlL.lib"
    "${OCC_LIB_DIR}/TKFillet.lib"
    "${OCC_LIB_DIR}/TKMeshVS.lib"
    "${OCC_LIB_DIR}/TKShHealing.lib"
    "${OCC_LIB_DIR}/TKV3d.lib"
    "${OCC_LIB_DIR}/TKXmlTObj.lib"
    "${OCC_LIB_DIR}/TKG2d.lib"
    "${OCC_LIB_DIR}/TKXmlXCAF.lib"
    "${OCC_LIB_DIR}/TKG3d.lib"
    "${OCC_LIB_DIR}/TKOffset.lib"
    "${OCC_LIB_DIR}/TKVRML.lib"
    "${OCC_LIB_DIR}/TKXSBase.lib"
    "${OCC_LIB_DIR}/TKGeomAlgo.lib"
    "${OCC_LIB_DIR}/TKOpenGl.lib"
    "${OCC_LIB_DIR}/TKSTEP.lib"
    "${OCC_LIB_DIR}/TKXCAF.lib"
    "${OCC_LIB_DIR}/TKGeomBase.lib"
    "${OCC_LIB_DIR}/TKSTEP209.lib"
    "${OCC_LIB_DIR}/TKHLR.lib"
    "${OCC_LIB_DIR}/TKSTEPAttr.lib"
    "${OCC_LIB_DIR}/TKXDEIGES.lib"
    "${OCC_LIB_DIR}/TKIGES.lib"
    "${OCC_LIB_DIR}/TKPrim.lib"
    "${OCC_LIB_DIR}/TKSTEPBase.lib"
    "${OCC_LIB_DIR}/TKXDESTEP.lib")

  target_include_directories(
    OCC
    INTERFACE
    ${OCC_INC_DIR}
    )
endif(Use_Linux_OS)
