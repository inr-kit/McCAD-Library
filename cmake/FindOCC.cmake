message("")
message(STATUS "====================")
message(STATUS "Locating OpenCascade")
message(STATUS "====================")
if(Linux_OS)
    find_package(OpenCASCADE 7.3.0 EXACT REQUIRED)
    if(OpenCASCADE_FOUND)
        message(STATUS "OpenCASCADE ${OpenCASCADE_VERSION} found")
        if(BUILD_SHARED)
            set(OCC_LIBRARIES_SHARED)
            set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_SHARED_LIBRARY_SUFFIX})
            foreach (OCC_lib ${OpenCASCADE_LIBRARIES})
                set(OCC_LIB "OCC_LIB-NOTFOUND")
                find_library(OCC_LIB
                    NAMES ${OCC_lib}
                    HINTS ${OpenCASCADE_LIBRARY_DIR}
                    NO_DEFAULT_PATH)
                list(APPEND OCC_LIBRARIES_SHARED ${OCC_LIB})
            endforeach ()
        endif()
        if(BUILD_STATIC)
            set(OCC_LIBRARIES_STATIC)
            set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
            foreach (OCC_lib ${OpenCASCADE_LIBRARIES})
                set(OCC_LIB "OCC_LIB-NOTFOUND")
                find_library(OCC_LIB
                    NAMES ${OCC_lib}
                    HINTS ${OpenCASCADE_LIBRARY_DIR}
                    NO_DEFAULT_PATH)
                list(APPEND OCC_LIBRARIES_STATIC ${OCC_LIB})
            endforeach ()
        endif()
        set(OCC_INCLUDE_DIRS ${OpenCASCADE_INCLUDE_DIR})
        set(OCC_LIBRARY_DIRS ${OpenCASCADE_LIBRARY_DIR})
    else(OpenCASCADE_FOUND)
        set(OCC_LIBRARIES TKernel TKMath TKG2d TKG3d TKGeomBase TKBRep TKGeomAlgo
            TKTopAlgo TKPrim TKBO TKShHealing TKBool TKHLR TKFillet TKOffset TKFeat
            TKMesh TKXMesh TKService TKV3d TKOpenGl TKMeshVS TKCDF TKLCAF TKCAF TKBinL
            TKXmlL TKBin TKXml TKStdL TKStd TKTObj TKBinTObj TKXmlTObj TKVCAF TKXSBase
            TKSTEPBase TKSTEPAttr TKSTEP209 TKSTEP TKIGES TKXCAF TKXDEIGES TKXDESTEP
            TKSTL TKVRML TKXmlXCAF TKBinXCAF TKDraw TKTopTest TKViewerTest TKXSDRAW
            TKDCAF TKXDEDRAW TKTObjDRAW TKQADraw)
        if(DEFINED ENV{CASROOT})
            if(NOT DEFINED OpenCASCADE_INCLUDE_DIR)
                set(OCC_INCLUDE_DIRS "$ENV{CASROOT}/include/opencascade")
            endif(NOT DEFINED OpenCASCADE_INCLUDE_DIR)
            if(NOT DEFINED OpenCASCADE_LIBRARY_DIR)
                set(OCC_LIBRARY_DIRS "$ENV{CASROOT}/lib")
            endif(NOT DEFINED OpenCASCADE_LIBRARY_DIR)
        else(DEFINED ENV{CASROOT})
            if(NOT DEFINED OpenCASCADE_INCLUDE_DIR OR NOT DEFINED OpenCASCADE_LIBRARY_DIR)
                message(WARNING "To specify paths of OpenCascade files, you may\n
                    either define the CASROOT environment variable, or set both\n
                    OCC_INCLUDE_DIRS and OCC_LIBRARY_DIRS variables.")
            endif(NOT DEFINED OpenCASCADE_INCLUDE_DIR OR NOT DEFINED OpenCASCADE_LIB_DIR)
        endif(DEFINED ENV{CASROOT})
    endif(OpenCASCADE_FOUND)

    if(DEFINED OCC_INCLUDE_DIRS)
        include_directories(${OCC_INCLUDE_DIRS})
    endif(DEFINED OCC_INCLUDE_DIRS)
else(Linux_OS)
    if(DEFINED OCC_ROOT)
        set(OCC_LIBRARY_DIRS "${OCC_ROOT}/win64/vc14/lib")
        set(OCC_INCLUDE_DIRS "${OCC_ROOT}/inc")
        set(OpenCASCADE_LIBRARIES   "${OCC_LIB_DIR}/TKBin.lib"
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

        if(BUILD_SHARED)
            set(OCC_LIBRARIES_SHARED)
            foreach (OCC_lib ${OpenCASCADE_LIBRARIES})
                set(OCC_LIB "OCC_LIB-NOTFOUND")
                find_library(OCC_LIB
                    NAMES ${OCC_lib}
                    HINTS ${OCC_LIBRARY_DIRS}
                    NO_DEFAULT_PATH)
                list(APPEND OCC_LIBRARIES_SHARED ${OCC_LIB})
            endforeach ()
        endif()
        if(BUILD_STATIC)
            set(OCC_LIBRARIES_STATIC)
            foreach (OCC_lib ${OpenCASCADE_LIBRARIES})
                set(OCC_LIB "OCC_LIB-NOTFOUND")
                find_library(OCC_LIB
                    NAMES ${OCC_lib}
                    HINTS ${OCC_LIBRARY_DIRS}
                    NO_DEFAULT_PATH)
                list(APPEND OCC_LIBRARIES_STATIC ${OCC_LIB})
            endforeach ()
        endif()

        include_directories(${OCC_INCLUDE_DIRS})
    endif(DEFINED OCC_ROOT)
endif(Linux_OS)

message(STATUS "OCC_INCLUDE_DIRS: ${OCC_INCLUDE_DIRS}")
message(STATUS "OCC_LIBRARY_DIRS: ${OCC_LIBRARY_DIRS}")
if (BUILD_SHARED)
    message(STATUS "OCC_LIBRARIES_SHARED: ${OCC_LIBRARIES_SHARED}")
else()
    message(STATUS "OCC_LIBRARIES_STATIC: ${OCC_LIBRARIES_STATIC}")
endif()

