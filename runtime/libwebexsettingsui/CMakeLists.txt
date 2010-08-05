project(libwebexsettingsui)
#cmake_minimum_required(VERSION 2.6)

find_package(KDE4  REQUIRED)
find_package(Nepomuk REQUIRED)
find_package(Soprano REQUIRED)

include_directories( 
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${libwebexsettings_BINARY_DIR}
    ${libwebexsettings_SOURCE_DIR}
    ${SOPRANO_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    )

add_definitions(-DMAKE_WEBEXSETTINGSUI_LIB)

set( webexsettingsui_SRCS
    mergepoliticsselector.cpp
    #pluginselector.cpp
    catnamevalidator.cpp
    decisionsmodelselector.cpp
    )

kde4_add_ui_files(webexsettingsui_SRCS
    decisionsModelSelector.ui
    )

kde4_add_library( webexsettingsui STATIC 
    ${webexsettingsui_SRCS}
    )

IF(UNIX AND NOT WIN32)
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
      SET_TARGET_PROPERTIES(webexsettingsui PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
ENDIF(UNIX AND NOT WIN32)


target_link_libraries( webexsettingsui 
    webexsettings
    ${SOPRANO_LIBRARIES}
    )


remove_definitions(-DMAKE_WEBEXSETTINGSUI_LIB)

