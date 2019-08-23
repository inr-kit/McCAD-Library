if(Use_Linux_OS)
  message("Linux")
  # This part is imported from https://github.com/tpaviot/oce/blob/master/examples/find_package_components/CMakeLists.txt
  # find_package(OCE) by specifying a list of toolkits.
  # If a toolkit is missing or if OCE is not found, try to find an OpenCascade installation.
  set(OCE_TOOLKITS FWOSPlugin TKCDF TKLCAF TKPShape TKSTL TKXMesh PTKernel TKernel TKMath
    TKService TKTObj TKXml TKBin TKFeat TKMesh TKShapeSchema TKTopAlgo TKXmlL TKBinL
    TKFillet TKMeshVS TKShHealing TKV3d TKXmlTObj TKBinTObj TKG2d TKNIS TKStdLSchema
    TKVoxel TKXmlXCAF TKBinXCAF TKG3d TKOffset TKStdSchema TKVRML TKXSBase TKBO
    TKGeomAlgo TKOpenGl TKSTEP TKXCAF TKBool TKGeomBase TKPCAF TKSTEP209 TKXCAFSchema
    TKBRep TKHLR TKPLCAF TKSTEPAttr TKXDEIGES TKCAF TKIGES TKPrim TKSTEPBase TKXDESTEP)
  
  find_package(OCE COMPONENTS ${OCE_TOOLKITS})
  if(OCE_FOUND)
    message(STATUS "Found OCE version ${OCE_VERSION}")
    if(NOT OCE_ALL_FOUND)
      set(OCE_FOUND false)
      message(WARNING "Ignoring OCE installation due to missing toolkit(s): ${OCE_MISSING_TOOLKITS}")
    endif(NOT OCE_ALL_FOUND)
  endif(OCE_FOUND)

  if(OCE_FOUND)
    # Include files reside in ${OCE_INCLUDE_DIRS};
    add_library(OCC_TOOLKITS INTERFACE)
    install (TARGETS OCC_TOOLKITS DESTINATION ${PROJECT_BINARY_DIR}/bin)
    target_link_libraries(OCC_TOOLKITS INTERFACE ${OCE_TOOLKITS})
    target_include_directories(OCC_TOOLKITS INTERFACE ${OCE_INCLUDE_DIRS})
    message(STATUS "  > OCE include_dir ${OCE_INCLUDE_DIRS}")
    # We do not need library path, they will be automatically imported.
  else(OCE_FOUND)
    # OCE not found; either it is not found and user
    # has to set OCE_DIR to the directory containing
    # OCEConfig.cmake, or OCE is not installed and we
    # try to find OpenCascade files.
    if(DEFINED ENV{CASROOT})
      if(NOT DEFINED OCC_INCLUDE_PATH)
        set(OCC_INCLUDE_PATH "$ENV{CASROOT}/inc")
      endif(NOT DEFINED OCC_INCLUDE_PATH)
      if(NOT DEFINED OCC_LIB_PATH)
        if(WIN32)
	  set(OCC_LIB_PATH "$ENV{CASROOT}/win32/lib")
        else(WIN32)
	  if(APPLE)
	    set(OCC_LIB_PATH "/Library/OpenCASCADE/6.3.0/lib")
	  else(APPLE)
	    set(OCC_LIB_PATH "$ENV{CASROOT}/lin/lib")
	  endif(APPLE)
        endif(WIN32)
      endif(NOT DEFINED OCC_LIB_PATH)
    else(DEFINED ENV{CASROOT})
      if(NOT DEFINED OCC_INCLUDE_PATH OR NOT DEFINED OCC_LIB_PATH)
        message(WARNING "To specify paths of OpenCascade files, you may either define the CASROOT environment variable, or set both OCC_INCLUDE_PATH and OCC_LIB_PATH variables.")
      endif(NOT DEFINED OCC_INCLUDE_PATH OR NOT DEFINED OCC_LIB_PATH)
    endif(DEFINED ENV{CASROOT})
    if(DEFINED OCC_INCLUDE_PATH)
      message(STATUS "OCC search path for include files: OCC_INCLUDE_PATH=${OCC_INCLUDE_PATH}")
      include_directories(${OCC_INCLUDE_PATH})
    endif(DEFINED OCC_INCLUDE_PATH)
    if(DEFINED OCC_LIB_PATH)
      message(STATUS "OCC search path for libraries: OCC_LIB_PATH=${OCC_LIB_PATH}")
      link_directories(${OCC_LIB_PATH})
    endif(DEFINED OCC_LIB_PATH)
  endif(OCE_FOUND)

## =========================================================

else(Use_Linux_OS)
  message("Non-Linux")
  add_library(OCC_TOOLKITS INTERFACE)

  set(OCC_LIB_DIR
    "C:/OpenCASCADE/OpenCASCADE-7.3.0-vc14-64/opencascade-7.3.0/win64/vc14/lib")
  set(OCC_INC_DIR
    "C:/OpenCASCADE/OpenCASCADE-7.3.0-vc14-64/opencascade-7.3.0/inc")

  target_link_libraries(OCC_TOOLKITS
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
    OCC_TOOLKITS
    INTERFACE
    ${OCC_INC_DIR}
    )
endif(Use_Linux_OS)
