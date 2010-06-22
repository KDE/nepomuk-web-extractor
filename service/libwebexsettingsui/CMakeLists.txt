project(libwebexsettingsui)
cmake_minimum_required(VERSION 2.6)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

add_definitions(-DMAKE_WEBEXSETTINGSUI_LIB)

set( webexsettingsui_SRCS
    mergepoliticsselector.cpp
    )

kde4_add_library( webexsettingsui STATIC 
    ${webexsettings_SRCS}
    )

IF(UNIX AND NOT WIN32)
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
      SET_TARGET_PROPERTIES(webexsettings PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
ENDIF(UNIX AND NOT WIN32)


target_link_libraries( webexsettingsui 
    webexsettings
    )


remove_definitions(-DMAKE_WEBEXSETTINGSUI_LIB)

