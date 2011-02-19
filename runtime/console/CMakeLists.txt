project(webextractor_console)

cmake_minimum_required(VERSION 2.6)

set (QT_MIN_VERSION "4.6.0")
set (QT_USE_QTNETWORK true)

#include(KDE4Defaults)
#include(MacroLibrary)

find_package(KDE4  REQUIRED)
find_package(Nepomuk REQUIRED)
# find_package(LibNepomuksearchgui REQUIRED)

# check for required packages
if(BUILD_METAPACKAGE)
	# if WEBEXTRACTOR_VERSION_STRING is defined, we are compiling the meta-package
	set(LIBWEBEXTRACTOR_LIBRARIES webextractor)
else(BUILD_METAPACKAGE)
	find_package(LibWebextractor REQUIRED)
endif(BUILD_METAPACKAGE)


include_directories(
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  #libwebexsettings path
  ${libwebexsettings_BINARY_DIR}
  ${libwebexsettings_SOURCE_DIR}
  #libwebexsettingsui path
  ${libwebexsettingsui_BINARY_DIR}
  ${libwebexsettingsui_SOURCE_DIR}
  ${NEPOMUK_INCLUDE_DIR}
  ${NEPOMUK_INCLUDE_DIR}/nepomuk
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${LIBWEBEXTRACTOR_INCLUDE_DIRS}
  ${LIBWEBEXTRACTORUI_INCLUDE_DIRS}
  )


link_directories(${LIBWEBEXTRACTOR_LIBRARY_DIRS})

set ( webexconsole_SRCS
    console.cpp
    main.cpp
    modeltest.cpp
    examinedwidget.cpp
    infoPage.cpp
    launchPage.cpp
    settingsPage.cpp
    )

kde4_add_ui_files(webexconsole_SRCS
   mainWindow.ui
   launchPage.ui
   settingsPage.ui
   infoPage.ui
  ) 
kde4_add_executable( webexconsole ${webexconsole_SRCS} )

target_link_libraries( webexconsole 
    webexsettings
    webexsettingsui
    webextractor
    webextractorui
    ${LIBWEBEXTRACTOR_LIBRARIES}
    ${NEPOMUK_LIBRARIES}
    ${KDE4_NEPOMUK_LIBS}
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KDEUI_LIBRARY}
    nepomukutils
    nepomuksync
    )

install( TARGETS webexconsole ${INSTALL_TARGETS_DEFAULT_ARGS} )
install(FILES webexconsole.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
