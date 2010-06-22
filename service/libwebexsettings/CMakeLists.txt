project(libwebexsettings)
cmake_minimum_required(VERSION 2.6)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

configure_file(settings_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/settings_config.h )

add_definitions(-DMAKE_WEBEXSETTINGS_LIB)

set( webexsettings_SRCS
    webextractor_config.cpp
    webexcatconfig.cpp
    datapppool.cpp
    datappconfig.cpp
    categoriespool.cpp
    global.cpp
    )
kde4_add_kcfg_files(webexsettings_SRCS webexconfigbase.kcfgc)
kde4_add_kcfg_files(webexsettings_SRCS webexcategory.kcfgc)
kde4_add_kcfg_files(webexsettings_SRCS datappconfigbase.kcfgc)

kde4_add_library( webexsettings STATIC 
    ${webexsettings_SRCS}
    )

IF(UNIX AND NOT WIN32)
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
      SET_TARGET_PROPERTIES(webexsettings PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
ENDIF(UNIX AND NOT WIN32)


target_link_libraries( webexsettings 
    webexplugin 
    )

install(
    FILES webextractor.kcfg DESTINATION ${KCFG_INSTALL_DIR}
    )

remove_definitions(-DMAKE_WEBEXSETTINGS_LIB)
