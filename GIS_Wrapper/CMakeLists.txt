set(PROJECT_NAME GIS_Abilities)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "GIS_Abilities.cd"
)
source_group("" FILES ${no_group_source_files})

set(Gen_file
    "moc_DelSymbol.cpp"
    "moc_DrawLineTP.cpp"
    "moc_DrawRectangle.cpp"
    "moc_DrawTexSymbol.cpp"
    "moc_GraticuleCoordinates.cpp"
    "moc_Layer.cpp"
    "moc_ManualDraw.cpp"
    "moc_ModifyContour.cpp"
    "moc_ModifyStationSymbol.cpp"
    "moc_ModifySymbol.cpp"
    "moc_ScaleSymbolLine.cpp"
    "moc_SymbolLayer.cpp"
    "moc_TextBox.cpp"
    "moc_VectorLayer.cpp"
)
source_group("Gen file" FILES ${Gen_file})

set(Header_Files
    "AlgorithmInterface.h"
    "AnalysisExponent.h"
    "AnalysisStationFile.h"
    "CIPASException.h"
    "CipasField.h"
    "CIPASMessageBox.h"
    "CIPScene.h"
    "Circlip.h"
    "CLineAnalysis.h"
    "ColorCardLegend.h"
    "ConcavePolygon.h"
    "Const.h"
    "ContourData.h"
    "ContourLayer.h"
    "ContourLegend.h"
    "ContourShadedLayer.h"
    "ConvertExponentToCipasFormat.h"
    "CoordinateTransform.h"
    "CPolygonAnalysis.h"
    "CustomizeArea.h"
    "DataProvider.h"
    "DelSymbol.h"
    "dem.h"
    "DiskFileLog.h"
    "DrawArea.h"
    "DrawContourArea.h"
    "DrawContourLine.h"
    "DrawCricle.h"
    "DrawLineDistance.h"
    "DrawLineSymbol.h"
    "DrawLineTP.h"
    "DrawMicapsGroundMap.h"
    "DrawNormalSymbol.h"
    "DrawOrder.h"
    "DrawPointSymbol.h"
    "DrawRectangle.h"
    "DrawShapeSymbol.h"
    "DrawString.h"
    "DrawTexSymbol.h"
    "DrawTroughLine.h"
    "DrawWindSymbol.h"
    "EarthSphere.h"
    "FunPluginInterface.h"
    "GeoCoordTransform.h"
    "Geometry.h"
    "GeoTo3DTransform.h"
    "GIS_WrapperConfig.h"
    "gpc.h"
    "GraticuleCoordinates.h"
    "ILayerAttribute.h"
    "ImageTexture.h"
    "Layer.h"
    "LayerDisplayControl.h"
    "Legend.h"
    "Line.h"
    "Log.h"
    "ManualDraw.h"
    "Map.h"
    "MeasureMouseManipulator.h"
    "ModifyContour.h"
    "ModifyStationSymbol.h"
    "ModifySymbol.h"
    "NormalSymbol.h"
    "OptMicapsDataXml.h"
    "PixmapTexture.h"
    "Plane.h"
    "Point.h"
    "Polygon.h"
    "Proj4Transform.h"
    "RadProjTransform.h"
    "ReadColorTableFromXML.h"
    "ReadSymbolFromM14File.h"
    "SaveColorTableToXML.h"
    "SaveLayerToKML.h"
    "SaveLayerToShape.h"
    "SaveSymbolToM14File.h"
    "SaveSymbolToXML.h"
    "ScaleSymbolLine.h"
    "shapefil.h"
    "StationInterpretation.h"
    "Symbol.h"
    "SymbolAttribute.h"
    "SymbolContourLine.h"
    "SymbolData.h"
    "SymbolLayer.h"
    "SymbolLine.h"
    "SymbolMultiContourLine.h"
    "SymbolMultiLine.h"
    "SymbolPluginInterface.h"
    "SymbolPoint.h"
    "TextBox.h"
    "tin.h"
    "VectorLayer.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "AlgorithmInterface.cpp"
    "AnalysisExponent.cpp"
    "AnalysisStationFile.cpp"
    "CIPASException.cpp"
    "CipasField.cpp"
    "CIPASMessageBox.cpp"
    "CIPScene.cpp"
    "Circlip.cpp"
    "CLineAnalysis.cpp"
    "ColorCardLegend.cpp"
    "ConcavePolygon.cpp"
    "ContourData.cpp"
    "ContourLayer.cpp"
    "ContourLegend.cpp"
    "ContourShadedLayer.cpp"
    "ConvertExponentToCipasFormat.cpp"
    "CoordinateTransform.cpp"
    "CPolygonAnalysis.cpp"
    "CustomizeArea.cpp"
    "DataProvider.cpp"
    "DelSymbol.cpp"
    "DiskFileLog.cpp"
    "DrawArea.cpp"
    "DrawContourArea.cpp"
    "DrawContourLine.cpp"
    "DrawCricle.cpp"
    "DrawLineDistance.cpp"
    "DrawLineSymbol.cpp"
    "DrawLineTP.cpp"
    "DrawMicapsGroundMap.cpp"
    "DrawNormalSymbol.cpp"
    "DrawOrder.cpp"
    "DrawPointSymbol.cpp"
    "DrawRectangle.cpp"
    "DrawShapeSymbol.cpp"
    "DrawString.cpp"
    "DrawTexSymbol.cpp"
    "DrawWindSymbol.cpp"
    "EarthSphere.cpp"
    "GeoCoordTransform.cpp"
    "Geometry.cpp"
    "GeoTo3DTransform.cpp"
    "gpc.cpp"
    "GraticuleCoordinates.cpp"
    "ILayerAttribute.cpp"
    "ImageTexture.cpp"
    "Layer.cpp"
    "LayerDisplayControl.cpp"
    "Legend.cpp"
    "Line.cpp"
    "Log.cpp"
    "ManualDraw.cpp"
    "Map.cpp"
    "MeasureMouseManipulator.cpp"
    "ModifyContour.cpp"
    "ModifyStationSymbol.cpp"
    "ModifySymbol.cpp"
    "NormalSymbol.cpp"
    "OptMicapsDataXml.cpp"
    "PixmapTexture.cpp"
    "Plane.cpp"
    "Point.cpp"
    "Polygon.cpp"
    "Proj4Transform.cpp"
    "RadProjTransform.cpp"
    "ReadColorTableFromXML.cpp"
    "ReadSymbolFromM14File.cpp"
    "SaveColorTableToXML.cpp"
    "SaveLayerToKML.cpp"
    "SaveLayerToShape.cpp"
    "SaveSymbolToM14File.cpp"
    "SaveSymbolToXML.cpp"
    "ScaleSymbolLine.cpp"
    "Symbol.cpp"
    "SymbolAttribute.cpp"
    "SymbolContourLine.cpp"
    "SymbolData.cpp"
    "SymbolLayer.cpp"
    "SymbolLine.cpp"
    "SymbolMultiContourLine.cpp"
    "SymbolMultiLine.cpp"
    "SymbolPoint.cpp"
    "TextBox.cpp"
    "tin.cpp"
    "VectorLayer.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(nn
    "nn/delaunay.c"
    "nn/delaunay.h"
    "nn/hash.c"
    "nn/hash.h"
    "nn/istack.c"
    "nn/istack.h"
    "nn/lpi.c"
    "nn/nan.h"
    "nn/nn.h"
    "nn/nnai.c"
    "nn/nncommon.c"
    "nn/nnpi.c"
    "nn/triangle.c"
    "nn/triangle.h"
    "nn/version.h"
)
source_group("nn" FILES ${nn})

set(ALL_FILES
    ${no_group_source_files}
    ${Gen_file}
    ${Header_Files}
    ${Source_Files}
    ${nn}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GIS_Wrapper)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../Release/Bin"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG;"
        "CDATAELEMENT_LIB;"
        "_MBCS"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "UNICODE;"
        "_UNICODE"
    ">"
    "WIN32;"
    "_LIB;"
    "QE_LIB_IMPORT"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /w
        >
        $<$<CONFIG:Release>:
            /O2;
            /Oi;
            /Gy;
            /W3
        >
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Zc:wchar_t-;
        /Y-
    )
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
        "QtNetworkd4;"
        "QtGuid4;"
        "QtCored4;"
        "QtOpenGLd4"
    ">"
    "$<$<CONFIG:Release>:"
        "XMLProvider;"
        "QtGui4;"
        "QtCore4;"
        "QtOpenGL4;"
        "qgis_core;"
        "qgis_gui;"
        "CDataElement;"
        "CDataProviderManager;"
        "QtNetwork4"
    ">"
    "glu32;"
    "opengl32;"
    "proj_i;"
    "gdal_i;"
    "QE_lib"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

