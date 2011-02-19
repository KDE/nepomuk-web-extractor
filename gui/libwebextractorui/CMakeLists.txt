project(libwebextractorui)

cmake_minimum_required(VERSION 2.6)

set (QT_MIN_VERSION "4.6.0")
set (QT_USE_QTNETWORK true)


find_package(Qt4  REQUIRED)
find_package(KDE4  REQUIRED)
find_package(Nepomuk REQUIRED)

# check for required packages
if(BUILD_METAPACKAGE)
	# if WEBEXTRACTOR_VERSION_STRING is defined, we are compiling the meta-package
	set(LIBWEBEXTRACTOR_LIBRARIES webextractor)
else(BUILD_METAPACKAGE)
	find_package(LibWebextractor REQUIRED)
endif(BUILD_METAPACKAGE)

include(KDE4Defaults)
include(MacroLibrary)

set(WEBEXTRACTORUI_VERSION_MAJOR 0 CACHE INT "Major WebExtractor UI version number" FORCE)
set(WEBEXTRACTORUI_VERSION_MINOR 0 CACHE INT "Minor WebExtractor UI version number" FORCE)
set(WEBEXTRACTORUI_VERSION_PATCH 1 CACHE INT "Release WebExtractor UI version number" FORCE)
set(WEBEXTRACTORUI_VERSION_STRING "${WEBEXTRACTOR_VERSION_MAJOR}.${WEBEXTRACTOR_VERSION_MINOR}.${WEBEXTRACTOR_VERSION_PATCH}" CACHE STRING "Webextractor version string" FORCE)
set( LIBWEBEXTRACTORUI_VERSION $WEBEXTRACTORUI_VERSION_STRING})

# definition of LIB_DESTINATION which is the path where the lib is installed
set (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
set (LIB_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING "Library directory name" FORCE)

configure_file(LibWebextractoruiConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/LibWebextractoruiConfig.cmake @ONLY )

include_directories(
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${NEPOMUK_INCLUDE_DIR}
  ${NEPOMUK_INCLUDE_DIR}/nepomuk
  ${CMAKE_BUILD_DIR}
  ${KDE4_INCLUDES}
  ${LIBWEBEXTRACTOR_INCLUDE_DIRS}
  ${backupsynclegacy_SOURCE_DIR}
  )

add_definitions(-DMAKE_WEBEXTRACTORUI_LIB)
set( webextractorui_SRCS 
    changelogwidget.cpp
    decisionwidget.cpp
    decisioncollectionwidget.cpp
    )

set (webextractorui_HEADERS 
    webextractorui_export.h
    changelogwidget.h
    decisionwidget.h
    decisioncollectionwidget.h
    ) 

set( webextractorui_UI
    changeLogForm.ui
    decisionForm.ui
    )


kde4_add_ui_files( webextractorui_SRCS
   ${webextractorui_UI}
  )


kde4_add_library( webextractorui SHARED ${webextractorui_SRCS} ${backupsynclegacy_SRC})


set_target_properties(webextractorui PROPERTIES
	VERSION ${WEBEXTRACTORUI_VERSION_STRING}
	SOVERSION ${WEBEXTRACTORUI_VERSION_MAJOR}
	DEFINE_SYMBOL MAKE_WEBEXTRACTORUI_LIB
	)

target_link_libraries( webextractorui 
  ${NEPOMUK_LIBRARIES}
  ${NEPOMUK_QUERY_LIBRARIES}
  ${KDE4_NEPOMUK_LIBS}
  ${QT_LIBRARIES}
  ${LIBWEBEXTRACTOR_LIBRARIES}
  nepomuksynclegacy
  )

install(TARGETS webextractorui
    	LIBRARY DESTINATION ${LIB_DESTINATION}
    	RUNTIME DESTINATION bin
    	ARCHIVE DESTINATION ${LIB_DESTINATION}
	)
remove_definitions(-DMAKE_WEBEXTRACTORUI_LIB)


#Qt designer plugins
set (decisionwidgetdesignerplugin_SRCS
    decisionwidgetdesignerplugin.cpp
    )
kde4_add_plugin( decisionwidgetdesignerplugin 
    ${decisionwidgetdesignerplugin_SRCS}
    )

target_link_libraries( decisionwidgetdesignerplugin
	${QT_LIBRARIES}
	${KDE4_KIO_LIBS}
	nepomuksynclegacy
	webextractorui
	)

#installing

#set ( WEBEXTRACTOR_HEADERS 
#    ${CMAKE_CURRENT_SOURCE_DIR}/include/webextractor/resourceanalyzerfactory.h 
#    ${CMAKE_CURRENT_SOURCE_DIR}/include/webextractor/resourceanalyzer.h
#    )

install(FILES
	${webextractorui_HEADERS}
	DESTINATION ${INCLUDE_INSTALL_DIR}/webextractor
	)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LibWebextractoruiConfig.cmake
	DESTINATION ${LIB_DESTINATION}/libwebextractorui
	)


install( TARGETS decisionwidgetdesignerplugin  DESTINATION ${PLUGIN_INSTALL_DIR}/plugins/designer )
