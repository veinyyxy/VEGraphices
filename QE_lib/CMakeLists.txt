set(PROJECT_NAME QE_lib)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "QE_lib.cd"
    "ReadMe.txt"
)
source_group("" FILES ${no_group_source_files})

set(Header_Files
    "CardinalSpline.h"
    "ColorAttribute.h"
    "Config.h"
    "DivisionData.h"
    "DrawSurface.h"
    "GL_Include.h"
    "GLTransform.h"
    "LineStippleAttribute.h"
    "LineWidthAttribute.h"
    "Nccclosedcontour.h"
    "NccPoint.h"
    "Nccstation.h"
    "PixelBufferSurface.h"
    "PointSizeAttribute.h"
    "ProjectionTransform.h"
    "QAccessor.h"
    "QAttribute.h"
    "QAttributeSet.h"
    "QBitmapText.h"
    "QCamera.h"
    "QDataArray.h"
    "QDefaultMouseManipulator.h"
    "QERenderContext.h"
    "QEText.h"
    "QFork.h"
    "QLeaf.h"
    "QMouseManipulator.h"
    "QMouseManipulatorExtand.h"
    "QNode.h"
    "QNodeCallBack.h"
    "QObserver.h"
    "QPickup.h"
    "QPixmapTextrue.h"
    "QRenderable.h"
    "QRenderUnit.h"
    "QRenderVolume.h"
    "QRenderWindow.h"
    "QScene.h"
    "QSceneCallBack.h"
    "QSceneManager.h"
    "QState.h"
    "QStateSet.h"
    "QT_Include.h"
    "QTesselator.h"
    "QTextrueInterface.h"
    "QTexture.h"
    "QTransform3D.h"
    "QViewport.h"
    "StationValues.h"
    "SysDefine.h"
    "TextureAttribute.h"
    "ViewModelTransform.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "CardinalSpline.cpp"
    "ColorAttribute.cpp"
    "DivisionData.cpp"
    "DrawSurface.cpp"
    "GLTransform.cpp"
    "LineStippleAttribute.cpp"
    "LineWidthAttribute.cpp"
    "Nccclosedcontour.cpp"
    "Nccstation.cpp"
    "PixelBufferSurface.cpp"
    "PointSizeAttribute.cpp"
    "ProjectionTransform.cpp"
    "QAccessor.cpp"
    "QAttribute.cpp"
    "QAttributeSet.cpp"
    "QBitmapText.cpp"
    "QCamera.cpp"
    "QDefaultMouseManipulator.cpp"
    "QERenderContext.cpp"
    "QEText.cpp"
    "QFork.cpp"
    "QLeaf.cpp"
    "QMouseManipulator.cpp"
    "QMouseManipulatorExtand.cpp"
    "QNode.cpp"
    "QNodeCallBack.cpp"
    "QObserver.cpp"
    "QPickup.cpp"
    "QPixmapTextrue.cpp"
    "QRenderable.cpp"
    "QRenderUnit.cpp"
    "QRenderVolume.cpp"
    "QRenderWindow.cpp"
    "QScene.cpp"
    "QSceneCallBack.cpp"
    "QSceneManager.cpp"
    "QState.cpp"
    "QStateSet.cpp"
    "QTesselator.cpp"
    "QTextrueInterface.cpp"
    "QTexture.cpp"
    "QTransform3D.cpp"
    "QViewport.cpp"
    "StationValues.cpp"
    "TextureAttribute.cpp"
    "ViewModelTransform.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${no_group_source_files}
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
################################################################################
# Includes for CMake from *.props
################################################################################
use_props(${PROJECT_NAME} Debug   "CIPAS_QT.cmake")

set(ROOT_NAMESPACE QE_lib)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_RELEASE "Bin${TARGET_NAME}$ENV{TargetExt}"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../Release"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Release>:
        MultiThreadedDLL
    >
    $<$<NOT:$<OR:$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "WIN32;"
    "_LIB;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /Ob0
        >
        $<$<CONFIG:Release>:
            /O2;
            /Oi;
            /Gy
        >
        /W3;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Y-
    )
    string(CONCAT FILE_CL_OPTIONS
        "$<$<CONFIG:Debug>:"
            "/Od;/RTC1;/ZI"
        ">"
    )
    source_file_compile_options(QStateSet.cpp ${FILE_CL_OPTIONS})
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /MANIFEST:NO;
        /DEBUG;
        /SUBSYSTEM:WINDOWS
    )
endif()

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        "QtGuid4;"
        "QtCored4;"
        "QtOpenGLd4;"
        "QtXmld4"
    ">"
    "$<$<CONFIG:Release>:"
        "QtGui4;"
        "QtCore4;"
        "QtOpenGL4;"
        "QtXml4"
    ">"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

