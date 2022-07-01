set(PROJECT_NAME GraphicsDemo)

################################################################################
# Source groups
################################################################################
set(Generated_Files
    "MyWidget.h"
    "Sphere.h"
)
source_group("Generated Files" FILES ${Generated_Files})

set(Source_Files
    "main.cpp"
    "MyWidget.cpp"
    "Sphere.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Generated_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

#use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE simpleax)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Qt4VSv1.0"
)
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "simpleax"
    TARGET_NAME_RELEASE "simpleax"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/release"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebugDLL
    >
    $<$<CONFIG:Release>:
        MultiThreadedDLL
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<$<CONFIG:Debug>:"
        "$ENV{CIPAS_QT}/include/QtCore;"
        "$ENV{CIPAS_QT}/include/QtGui;"
        "$ENV{CIPAS_QT}/include;"
        "$ENV{CIPAS_QT}/include/ActiveQt;"
        "$ENV{CIPAS_OSGeo4W}/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../Project/VEGraphices/QE_lib;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../Project/VEGraphices/GIS_Wrapper;"
        "${CMAKE_CURRENT_SOURCE_DIR}/tmp/moc/debug_shared"
    ">"
    "$<$<CONFIG:Release>:"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/QtCore";"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/QtGui";"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include";"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/ActiveQt";"
        "${CMAKE_CURRENT_SOURCE_DIR}/"tmp/moc/release_shared";"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../mkspecs/win32-msvc2010"
    ">"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "QE_LIB_IMPORT;"
        "GIS_WRAPPER_IMPORT"
    ">"
    "$<$<CONFIG:Release>:"
        "QT_NO_DEBUG;"
        "NDEBUG;"
        "QT_NO_DEBUG;"
        "NDEBUG"
    ">"
    "_WINDOWS;"
    "UNICODE;"
    "WIN32;"
    "QT_LARGEFILE_SUPPORT;"
    "QAXSERVER;"
    "QT_DLL;"
    "QT_GUI_LIB;"
    "QT_CORE_LIB;"
    "QT_THREAD_SUPPORT"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /Zi
        >
        $<$<CONFIG:Release>:
            /O2;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        /w;
        /GR;
        -Zm200;
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /GS-;
        /Zc:wchar_t-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:Release>:
            /SUBSYSTEM:WINDOWS;
            /INCREMENTAL:NO
        >
        /NOLOGO;
        "/MANIFESTDEPENDENCY:type='win32';
        name='Microsoft.Windows.Common-Controls';
        version='6.0.0.0';
        publicKeyToken='6595b64144ccf1df';
        language='*';
        processorArchitecture='*'"
    )
endif()

################################################################################
# Post build events
################################################################################
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND $<CONFIG:Release> C:\\Qt\\qt_vs2010\\qt\\bin\\idc.exe $<SHELL_PATH:$<TARGET_FILE:${PROJECT_NAME}>> /idl ${CMAKE_CURRENT_BINARY_DIR}\\${CMAKE_CFG_INTDIR}\\simpleax.idl -version 1.0
    COMMAND $<CONFIG:Release> midl ${CMAKE_CURRENT_BINARY_DIR}\\${CMAKE_CFG_INTDIR}\\simpleax.idl /nologo /tlb ${CMAKE_CURRENT_BINARY_DIR}\\${CMAKE_CFG_INTDIR}\\simpleax.tlb
    COMMAND $<CONFIG:Release> C:\\Qt\\qt_vs2010\\qt\\bin\\idc.exe $<SHELL_PATH:$<TARGET_FILE:${PROJECT_NAME}>> /tlb ${CMAKE_CURRENT_BINARY_DIR}\\${CMAKE_CFG_INTDIR}\\simpleax.tlb
    COMMAND $<CONFIG:Release> C:\\Qt\\qt_vs2010\\qt\\bin\\idc.exe $<SHELL_PATH:$<TARGET_FILE:${PROJECT_NAME}>> /regserver
    COMMENT "C:\\Qt\\qt_vs2010\\qt\\bin\\idc.exe $(TargetPath) /idl $(IntDir)simpleax.idl -version 1.0 midl $(IntDir)simpleax.idl /nologo /tlb $(IntDir)simpleax.tlb C:\\Qt\\qt_vs2010\\qt\\bin\\idc.exe $(TargetPath) /tlb $(IntDir)simpleax.tlb C:\\Qt\\qt_vs2010\\qt\\bin\\idc.exe $(TargetPath) /regserver"
)

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        "c:\\Qt\\qt_vs2010\\qt\\lib\\QtGuid4;"
        "c:\\Qt\\qt_vs2010\\qt\\lib\\QtCored4;"
        "C:\\Project\\VEGraphices\\Debug\\GIS_Abilities;"
        "C:\\Project\\VEGraphices\\Debug\\QE_lib;"
        "proj_i;"
        "gdal_i"
    ">"
    "$<$<CONFIG:Release>:"
        "c:\\Qt\\qt_vs2010\\qt\\lib\\QAxServer;"
        "c:\\Qt\\qt_vs2010\\qt\\lib\\QtGui4;"
        "c:\\Qt\\qt_vs2010\\qt\\lib\\QtCore4;"
        "user32;"
        "ole32;"
        "oleaut32;"
        "gdi32"
    ">"
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "$ENV{CIPAS_OSGeo4W}/lib"
    ">"
    "c:/Qt/qt_vs2010/qt/lib"
)

