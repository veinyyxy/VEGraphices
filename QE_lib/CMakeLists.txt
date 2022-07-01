set(PROJECT_NAME QE_lib)

include_directories(${CMAKE_PREFIX_PATH}/include)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
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
find_package(Qt6 REQUIRED COMPONENTS Core gui widgets opengl)
qt_standard_project_setup()

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

#use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
################################################################################
# Includes for CMake from *.props
################################################################################
#use_props(${PROJECT_NAME} Debug   "CIPAS_QT.cmake")

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

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::OpenGL Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGLWidgets opengl32 glu32)
#target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Opengl Qt6::gui)

