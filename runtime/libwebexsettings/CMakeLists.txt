## If you are going to make export this library, then all
## includes of headers from libwebextractor MUST be in the
## form <webextractor/XXX.h>.
## Don't forget to change kcfg files too.
project(libwebexsettings)
cmake_minimum_required(VERSION 2.6)

set (QT_MIN_VERSION "4.6.0")
set (QT_USE_QTNETWORK true)

#include(KDE4Defaults)
#include(MacroLibrary)

find_package(Qt4  REQUIRED)
find_package(KDE4  REQUIRED)
find_package(Nepomuk REQUIRED)

# check for required packages
if(WEBEXTRACTOR_VERSION_STRING)
	# if WEBEXTRACTOR_VERSION_STRING is defined, we are compiling the meta-package
	set(LIBWEBEXTRACTOR_INCLUDE_DIRS
		${libwebextractor_SOURCE_DIR})
	set(LIBWEBEXTRACTOR_LIBRARIES webextractor)
else(WEBEXTRACTOR_VERSION_STRING)
	find_package(LibWebextractor REQUIRED)
endif(WEBEXTRACTOR_VERSION_STRING)

# definition of LIB_DESTINATION which is the path where the lib is installed
set (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
set (LIB_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING "Library directory name" FORCE)

include_directories( 
	${CMAKE_CURRENT_BINARY_DIR} 
	${KDE4_INCLUDES}
	${NEPOMUK_INCLUDE_DIR}
	${QT_INCLUDE_DIR}
	${LIBWEBEXTRACTOR_INCLUDE_DIRS}
    )

configure_file(settings_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/settings_config.h )

add_definitions(-DMAKE_WEBEXSETTINGS_LIB)

set( webexsettings_SRCS
    webextractor_config.cpp
    #webexcatconfig.cpp
    datapppool.cpp
    datapp.cpp
    categoriespool.cpp
    global.cpp
    webextractor_kcm.cpp
    category.cpp
    datappdescr.cpp
    )

set( webexplugin_SRCS
        webextractor_plugin.cpp
    )

kde4_add_kcfg_files(webexsettings_SRCS webexconfigbase.kcfgc)
#kde4_add_kcfg_files(webexsettings_SRCS webexcategory.kcfgc)
kde4_add_kcfg_files(webexsettings_SRCS datappconfigbase.kcfgc)

kde4_add_library( webexsettings STATIC 
    ${webexsettings_SRCS}
    )

kde4_add_library ( webexplugin SHARED 
    ${webexplugin_SRCS} 
    )

IF(UNIX AND NOT WIN32)
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
      SET_TARGET_PROPERTIES(webexsettings PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
ENDIF(UNIX AND NOT WIN32)

target_link_libraries( webexplugin
    ${QT_QTCORE_LIBRARY}
   )


target_link_libraries( webexsettings 
    webexplugin 
    ${QT_LIBRARIES}
    ${NEPOMUK_QUERY_LIBRARIES}
    )

install(
    FILES webextractor.kcfg DESTINATION ${KCFG_INSTALL_DIR}
    )

install(TARGETS webexplugin
    	LIBRARY DESTINATION ${LIB_DESTINATION}
    	RUNTIME DESTINATION bin
    	ARCHIVE DESTINATION ${LIB_DESTINATION}
)

install(
  FILES webextractor_plugin.desktop
  DESTINATION ${SERVICETYPES_INSTALL_DIR}
  )
install (
    FILES webextractor_plugin.h DESTINATION ${INCLUDE_INSTALL_DIR}/webextractor
    )
install (
    FILES webextractor_kcm.h DESTINATION ${INCLUDE_INSTALL_DIR}/webextractor
    )
remove_definitions(-DMAKE_WEBEXSETTINGS_LIB)
