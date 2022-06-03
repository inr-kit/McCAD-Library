message("")
message(STATUS "==========================================")
message(STATUS "Locating Open OpenCASCADE_FOUND Technology")
message(STATUS "==========================================")
if(LINUX_OS)
    if(OCC_CUSTOM_ROOT)
        set(OCC_LIBRARY_DIRS "${OCC_CUSTOM_ROOT}/lib")
        set(OCC_INCLUDE_DIRS "${OCC_CUSTOM_ROOT}/include/opencascade")
        set(OpenCASCADE_FOUND True)
    else(OCC_CUSTOM_ROOT)
        if(DEFINED ENV{CASROOT})
            set(OCC_LIBRARY_DIRS "$ENV{CASROOT}/lib")
            set(OCC_INCLUDE_DIRS "$ENV{CASROOT}/include/opencascade")
            set(OpenCASCADE_FOUND True)
        endif()
        #find_package(OpenCASCADE 7.5.0 EXACT REQUIRED)
        #set(OCC_INCLUDE_DIRS ${OpenCASCADE_INCLUDE_DIR})
        #set(OCC_LIBRARY_DIRS ${OpenCASCADE_LIBRARY_DIR})
    endif(OCC_CUSTOM_ROOT)

    if(OpenCASCADE_FOUND)
        include_directories(${OCC_INCLUDE_DIRS})
        set(OpenCASCADE_LIBRARIES  TKBin TKBinL TKBinTObj TKBinXCAF TKBO TKBool
                                   TKBRep TKCAF TKCDF TKLCAF TKSTL TKXMesh TKernel
                                   TKMath TKService TKTObj TKXml TKFeat TKMesh
                                   TKTopAlgo TKXmlL TKFillet TKMeshVS TKShHealing
                                   TKV3d TKXmlTObj TKG2d TKXmlXCAF TKG3d TKOffset
                                   TKVRML TKXSBase TKGeomAlgo TKOpenGl TKSTEP
                                   TKXCAF TKGeomBase TKSTEP209 TKHLR TKSTEPAttr
                                   TKXDEIGES TKIGES TKPrim TKSTEPBase TKXDESTEP)
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
    else(OpenCASCADE_FOUND)
        message(WARNING "To specify paths of OpenCascade files, you may\n
                         either define the CASROOT environment variable, or set\n
                         OCC_CUSTOM_ROOT CMake variables.")
    endif()

else(LINUX_OS)
    if(DEFINED OCC_CUSTOM_ROOT)
        set(OCC_LIBRARY_DIRS "${OCC_CUSTOM_ROOT}/win64/vc14/lib")
        set(OCC_INCLUDE_DIRS "${OCC_CUSTOM_ROOT}/inc")
        set(OpenCASCADE_LIBRARIES   TKBin TKBinL TKBinTObj TKBinXCAF TKBO TKBool TKBRep TKCAF TKCDF TKLCAF 
                                    TKSTL TKXMesh TKernel TKMath TKService TKTObj TKXml TKFeat TKMesh TKTopAlgo 
                                    TKXmlL TKFillet TKMeshVS TKShHealing TKV3d TKXmlTObj TKG2d TKXmlXCAF TKG3d 
                                    TKOffset TKVRML TKXSBase TKGeomAlgo TKOpenGl TKSTEP TKXCAF TKGeomBase TKSTEP209 
                                    TKHLR TKSTEPAttr TKXDEIGES TKIGES TKPrim TKSTEPBase TKXDESTEP)
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
    else()
        message(FATAL_ERROR "OCC_CUSTOM_ROOT is not defined!")
    endif()
endif(LINUX_OS)

message(STATUS "OCC_INCLUDE_DIRS: ${OCC_INCLUDE_DIRS}")
message(STATUS "OCC_LIBRARY_DIRS: ${OCC_LIBRARY_DIRS}")
if (BUILD_SHARED)
    message(STATUS "OCC_LIBRARIES_SHARED: ${OCC_LIBRARIES_SHARED}")
endif()
if (BUILD_STATIC)
    message(STATUS "OCC_LIBRARIES_STATIC: ${OCC_LIBRARIES_STATIC}")
endif()

