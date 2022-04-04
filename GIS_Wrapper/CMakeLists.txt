cmake_minimum_required(VERSION 2.8)

IF(DYNAMIC_OPENSCENEGRAPH)
    ADD_DEFINITIONS(-DOSGANIMATION_LIBRARY)
ELSE()
    ADD_DEFINITIONS(-DOSG_LIBRARY_STATIC)
ENDIF()

SET(LIB_NAME GIS_Abilities)
set(HEADER_PATH ./)
set(SOURCE_PATH ./) 

SET(CUSTOM_H
    ${HEADER_PATH}CoordinateTransform.h
    ${HEADER_PATH}CustomizeArea.h
	${HEADER_PATH}DataProvider.h
	${HEADER_PATH}DelSymbol.h
    ${HEADER_PATH}DrawContourLine.h
    ${HEADER_PATH}DrawLineSymbol.h
    ${HEADER_PATH}DrawMicapsGroundMap.h
    ${HEADER_PATH}DrawNormalSymbol.h
    ${HEADER_PATH}DrawOrder.h
    ${HEADER_PATH}DrawPointSymbol.h
    ${HEADER_PATH}DrawPolygonSymbol.h
    ${HEADER_PATH}DrawTexSymbol.h
    ${HEADER_PATH}DrawTroughLine.h
    ${HEADER_PATH}DrawWindSymbol.h
	${HEADER_PATH}EditLayer.h
	${HEADER_PATH}GraticuleCoordinates.h
	${HEADER_PATH}Layer.h
	${HEADER_PATH}Line.h
	${HEADER_PATH}ManualDraw.h
	${HEADER_PATH}ManualDrawPolygon.h 
	${HEADER_PATH}ManualSymbolLine.h	
	${HEADER_PATH}Map.h
	${HEADER_PATH}VectorLayer.h
	${HEADER_PATH}ModifyContour.h
	${HEADER_PATH}Plane.h
	${HEADER_PATH}Point.h
	${HEADER_PATH}Polygon.h
	${HEADER_PATH}Proj4Transform.h
	${HEADER_PATH}QGISProvider.h
	${HEADER_PATH}ReadSymbolFromM14File.h
	${HEADER_PATH}ReadSymbolFromXML.h
	${HEADER_PATH}SaveSymbolToM14File.h
    ${HEADER_PATH}SaveSymbolToXML.h
	${HEADER_PATH}Symbol.h
	${HEADER_PATH}SymbolContourLine.h
	${HEADER_PATH}SymbolData.h
	${HEADER_PATH}SymbolFactory.h
	${HEADER_PATH}SymbolLayer.h
	${HEADER_PATH}SymbolLibraryManager.h
	${HEADER_PATH}SymbolLine.h
	${HEADER_PATH}SymbolPoint.h
	${HEADER_PATH}TextBox.h	
	${HEADER_PATH}UndoAddDrawCommand.h
	${HEADER_PATH}UndoAddPointCommand.h
    ${HEADER_PATH}UndoContourEditCommand.h
	${HEADER_PATH}UndoSymbolLineMoveCommand.h
	${HEADER_PATH}UndoSymbolMarkMoveCommand.h
	${HEADER_PATH}UndoSymbolPointMoveCommand.h
	${HEADER_PATH}UndoDeleteDrawCommand.h
	${HEADER_PATH}UndoDelPointCommand.h
	)	

set(TARGET_H           
         CIPASException.h
         CIPASMessageBox.h
         ColorCardLegend.h
         ContourData.h
         ContourLayer.h
         ContourLegend.h
         ContourShadedLayer.h
         CoordinateTransform.h
         CustomizeArea.h
         DataProvider.h
         DelSymbol.h
         dem.h
         DiskFileLog.h
         DrawContourLine.h
         DrawLineSymbol.h
         DrawMicapsGroundMap.h
         DrawNormalSymbol.h
         DrawOrder.h
         DrawPointSymbol.h
         DrawPolygonSymbol.h
         DrawTexSymbol.h
         DrawTroughLine.h
         DrawWindSymbol.h
         EditLayer.h
         FunPluginInterface.h
         Geometry.h
         GIS_WrapperConfig.h
         GraticuleCoordinates.h
         ILayerAttribute.h
         Layer.h
         Legend.h
         Line.h
         Log.h
         ManualDraw.h
         ManualDrawPolygon.h
         ManualSymbolLine.h
         Map.h
         ModifyContour.h
         ModifySymbol.h       
         NormalSymbol.h
         OptMicapsDataXml.h
         Plane.h
         Point.h
         Polygon.h
         Proj4Transform.h
         QGISProvider.h
         ReadSymbolFromM14File.h
         ReadSymbolFromXML.h
         SaveSymbolToM14File.h
         SaveSymbolToXML.h
         StationInterpretation.h
         Symbol.h
         SymbolAttribute.h
         SymbolContourLine.h
         SymbolData.h
         SymbolFactory.h
         SymbolLayer.h
         SymbolLibraryManager.h
         SymbolLine.h   
         SymbolPluginInterface.h
         SymbolPoint.h
         TextBox.h
         tin.h
         UndoAddDrawCommand.h
         UndoAddPointCommand.h
         UndoContourEditCommand.h
         UndoDeleteDrawCommand.h
         UndoDelPointCommand.h
         UndoManualDrawCommand.h
         UndoPointDrawCommand.h
         UndoSymbolLineMoveCommand.h
         UndoSymbolMarkMoveCommand.h
         UndoSymbolPointMoveCommand.h
         VectorLayer.h                 
	)
set(TARGET_SRC
           CIPASException.cpp
           CIPASMessageBox.cpp
           ColorCardLegend.cpp        
           ContourData.cpp
           ContourLayer.cpp
           ContourLegend.cpp
           ContourShadedLayer.cpp
           CoordinateTransform.cpp
           CustomizeArea.cpp
           DataProvider.cpp
           DelSymbol.cpp
           DiskFileLog.cpp
           DrawContourLine.cpp
           DrawLineSymbol.cpp
           DrawMicapsGroundMap.cpp
           DrawNormalSymbol.cpp
           DrawOrder.cpp
           DrawPointSymbol.cpp
           DrawPolygonSymbol.cpp
           DrawTexSymbol.cpp
           DrawTroughLine.cpp
           DrawWindSymbol.cpp
           EditLayer.cpp
           Geometry.cpp
           GraticuleCoordinates.cpp
           ILayerAttribute.cpp
           Layer.cpp
           Legend.cpp
           Line.cpp
           Log.cpp
           ManualDraw.cpp
           ManualDrawPolygon.cpp
           ManualSymbolLine.cpp
           Map.cpp
           ModifyContour.cpp
           ModifySymbol.cpp      
           NormalSymbol.cpp
           OptMicapsDataXml.cpp
           Plane.cpp
           Point.cpp
           Polygon.cpp
           Proj4Transform.cpp
           QGISProvider.cpp
           ReadSymbolFromM14File.cpp
           ReadSymbolFromXML.cpp
           SaveSymbolToM14File.cpp
           SaveSymbolToXML.cpp
           StationInterpretation.cpp
           Symbol.cpp
           SymbolAttribute.cpp
           SymbolContourLine.cpp
           SymbolData.cpp
           SymbolFactory.cpp
           SymbolLayer.cpp
           SymbolLibraryManager.cpp
           SymbolLine.cpp
           SymbolPoint.cpp
           TextBox.cpp
           UndoAddDrawCommand.cpp
           UndoAddPointCommand.cpp
           UndoContourEditCommand.cpp
           UndoDeleteDrawCommand.cpp
           UndoDelPointCommand.cpp
           UndoManualDrawCommand.cpp
           UndoPointDrawCommand.cpp
           UndoSymbolLineMoveCommand.cpp
           UndoSymbolMarkMoveCommand.cpp
           UndoSymbolPointMoveCommand.cpp
           VectorLayer.cpp	
	)

QT4_WRAP_CPP(CUSTOM_H_MOC ${TARGET_H})

include_directories("../QE_lib")
include_directories("../CDataElement")
include_directories("../CDataProviderManager")
include_directories("../AlgBase")
include_directories("../PluginSymbol")

add_library(${LIB_NAME} SHARED 
	${TARGET_H} 
	${TARGET_SRC}
	${CUSTOM_H_MOC} 
	)

#target_link_libraries(${LIB_NAME} QtCored4)
#target_link_libraries(${LIB_NAME} QtGuid4)
#target_link_libraries(${LIB_NAME} QtOpenGLd4)
#target_link_libraries(${LIB_NAME} glu32)
#target_link_libraries(${LIB_NAME} QtXmld4)
#target_link_libraries(${LIB_NAME} opengl32)
#target_link_libraries(${LIB_NAME} ftgl_dynamic_MT_d)
#target_link_libraries(${LIB_NAME} libfreetype)
#target_link_libraries(${LIB_NAME} qgis_core)
#target_link_libraries(${LIB_NAME} qgis_gui)#
#target_link_libraries(${LIB_NAME} gdal_i) 
#target_link_libraries(${LIB_NAME} proj_i)
#target_link_libraries(${LIB_NAME} AlgBase)
#target_link_libraries(${LIB_NAME} QE_lib)
#target_link_libraries(${LIB_NAME} CDataProviderManager)
#target_link_libraries(${LIB_NAME} CDataElement)

set_property(
   TARGET ${LIB_NAME}
   PROPERTY COMPILE_DEFINITIONS CDATAELEMENT_LIB QE_LIB_IMPORT _LIB
   )
