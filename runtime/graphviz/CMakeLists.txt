project(nepomukgraphviz)

cmake_minimum_required(VERSION 2.6)

set (QT_MIN_VERSION "4.6.0")
set (QT_USE_QTNETWORK true)

#include(KDE4Defaults)
#include(MacroLibrary)

find_package(KDE4  REQUIRED)
find_package(Nepomuk REQUIRED)
# find_package(LibNepomuksearchgui REQUIRED)

# check for required packages
if(WEBEXTRACTOR_VERSION_STRING)
	# if WEBEXTRACTOR_VERSION_STRING is defined, we are compiling the meta-package
	set(LIBWEBEXTRACTOR_INCLUDE_DIRS
		../libwebextractor/include
		${CMAKE_CURRENT_BINARY_DIR}/../libwebextractor/include)
	set(LIBWEBEXTRACTOR_LIBRARIES webextractor)
else(WEBEXTRACTOR_VERSION_STRING)
	find_package(LibWebextractor REQUIRED)
endif(WEBEXTRACTOR_VERSION_STRING)


include_directories(
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  #libwebexsettings path
  ${libwebexsettings_BINARY_DIR}
  ${libwebexsettings_SOURCE_DIR}
  #libwebexsettingsui path
  ${libwebexsettingsui_BINARY_DIR}
  ${libwebexsettingsui_SORUCE_DIR}
  ${NEPOMUK_INCLUDE_DIR}
  ${NEPOMUK_INCLUDE_DIR}/nepomuk
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${LIBWEBEXTRACTOR_INCLUDE_DIRS}
  )


link_directories(${LIBWEBEXTRACTOR_LIBRARY_DIRS})

set( nepomuk_graphviz_SRCS
    main.cpp
    graphvizmainwindow.cpp
    )

kde4_add_ui_files(nepomuk_graphviz_SRCS 
    mainWindow.ui
    )
kde4_add_executable( nepomuk_graphviz ${nepomuk_graphviz_SRCS} )

target_link_libraries( nepomuk_graphviz 
    ${LIBWEBEXTRACTOR_LIBRARIES}
    ${NEPOMUK_LIBRARIES}
    ${KDE4_NEPOMUK_LIBS}
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KDEUI_LIBRARY}
    nepomuksearchgui
    )
install( TARGETS nepomuk_graphviz ${INSTALL_TARGETS_DEFAULT_ARGS} )