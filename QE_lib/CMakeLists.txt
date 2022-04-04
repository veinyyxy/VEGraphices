cmake_minimum_required(VERSION 2.8)

IF(DYNAMIC_OPENSCENEGRAPH)
    ADD_DEFINITIONS(-DOSGANIMATION_LIBRARY)
ELSE()
    ADD_DEFINITIONS(-DOSG_LIBRARY_STATIC)
ENDIF()

SET(LIB_NAME QE_Lib)
set(HEADER_PATH ./)
set(SOURCE_PATH ./)

SET(VOLUME_HEADER ${HEADER_PATH}QRenderVolume.h)
SET(UNIT_HEADER ${SOURCE_PATH}QRenderUnit.h)

QT4_WRAP_CPP(VOLUME_HEADER_MOC ${VOLUME_HEADER})
QT4_WRAP_CPP(UNIT_HEADER_MOC ${UNIT_HEADER})

set(TARGET_H 
        ${HEADER_PATH}CardinalSpline.h
        ${HEADER_PATH}ColorAttribute.h
        ${HEADER_PATH}Config.h
        ${HEADER_PATH}DivisionData.h 
        ${HEADER_PATH}FTtoQimage.h 
        ${HEADER_PATH}GLTransform.h
        ${HEADER_PATH}GL_Include.h
        ${HEADER_PATH}LineStippleAttribute.h
        ${HEADER_PATH}LineWidthAttribute.h
        ${HEADER_PATH}NccPoint.h
        ${HEADER_PATH}Nccstation.h
        ${HEADER_PATH}PointSizeAttribute.h
        ${HEADER_PATH}PolygonCut.h
        ${HEADER_PATH}ProjectionTransform.h
        ${HEADER_PATH}QAccessor.h
        ${HEADER_PATH}QAttribute.h
        ${HEADER_PATH}QAttributeSet.h
        ${HEADER_PATH}QBitmapText.h
        ${HEADER_PATH}QCamera.h
        ${HEADER_PATH}QDataArray.h
        ${HEADER_PATH}QDefaultMouseManipulator.h
        ${HEADER_PATH}QFork.h
        ${HEADER_PATH}QLeaf.h
        ${HEADER_PATH}QMouseManipulator.h
        ${HEADER_PATH}QNode.h
        ${HEADER_PATH}QNodeCallBack.h
        ${HEADER_PATH}QNodeFactory.h
        ${HEADER_PATH}QObserver.h
        ${HEADER_PATH}QPickup.h
        ${HEADER_PATH}QRenderable.h
        ${HEADER_PATH}QRenderUnit.h
        ${HEADER_PATH}QRenderVolume.h
        ${HEADER_PATH}QRenderWindow.h
        ${HEADER_PATH}QScene.h
        ${HEADER_PATH}QSceneCallBack.h
        ${HEADER_PATH}QSceneManager.h
        ${HEADER_PATH}QState.h
        ${HEADER_PATH}QStateSet.h
        ${HEADER_PATH}QTesselator.h
        ${HEADER_PATH}QTexture.h
        ${HEADER_PATH}QTextureFont.h
        ${HEADER_PATH}QTransform3D.h
        ${HEADER_PATH}QT_Include.h
        ${HEADER_PATH}QViewport.h
        ${HEADER_PATH}StationValues.h
        ${HEADER_PATH}SysDefine.h
        ${HEADER_PATH}TextureAttribute.h
        ${HEADER_PATH}toolbox_interface.h
        ${HEADER_PATH}ViewModelTransform.h  
        ${VOLUME_HEADER_MOC}
        ${UNIT_HEADER_MOC}
	)
set(TARGET_SRC	
        CardinalSpline.cpp
        ColorAttribute.cpp
        DivisionData.cpp   
        FTtoQimage.cpp 
        GLTransform.cpp
        LineStippleAttribute.cpp
        LineWidthAttribute.cpp
        Nccstation.cpp
        PointSizeAttribute.cpp
        PolygonCut.cpp
	ProjectionTransform.cpp
        QAccessor.cpp
        QAttribute.cpp
        QAttributeSet.cpp
        QBitmapText.cpp
        QCamera.cpp
        QDefaultMouseManipulator.cpp
        QFork.cpp
        QLeaf.cpp
        QMouseManipulator.cpp
        QNode.cpp
        QNodeCallBack.cpp
        QNodeFactory.cpp
        QObserver.cpp
        QPickup.cpp
        QRenderable.cpp
        QRenderUnit.cpp
        QRenderVolume.cpp
        QRenderWindow.cpp
        QScene.cpp
        QSceneCallBack.cpp
        QSceneManager.cpp
        QState.cpp
        QStateSet.cpp
        QTesselator.cpp
        QTexture.cpp
        QTextureFont.cpp
        QTransform3D.cpp
        QViewport.cpp
        StationValues.cpp
        TextureAttribute.cpp
        ViewModelTransform.cpp
	)

add_library(QE_Lib SHARED 
	${TARGET_H} 
	${TARGET_SRC}
	)

##FIND_PACKAGE(ftgl REQUIRED)
##IND_PACKAGE(QT4 REQUIRED)

target_link_libraries(QE_Lib libQtCore.so libQtGui.so libQtXml.so libQtOpenGL.so libftgl.a libfreetype.a libglut.a )


