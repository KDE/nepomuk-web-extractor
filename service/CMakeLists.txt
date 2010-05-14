project(webextractorservice)

cmake_minimum_required(VERSION 2.6)

set (QT_MIN_VERSION "4.6.0")

#include(KDE4Defaults)
#include(MacroLibrary)

find_package(KDE4  REQUIRED)
find_package(Nepomuk REQUIRED)

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
  ${NEPOMUK_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${LIBWEBEXTRACTOR_INCLUDE_DIRS}
  )


link_directories(${LIBWEBEXTRACTOR_LIBRARY_DIRS})


set( webexservice_SRCS 
    webexservice.cpp
    webexscheduler.cpp
    priority.cpp
    useractivitymonitor.cpp
    webextractor_settings.cpp
    )

qt4_add_dbus_adaptor(webexservice_SRCS  org.kde.nepomuk.WebExtractorService.xml webexservice.h Nepomuk::WebExtractorService )



kde4_add_kcfg_files(webexservice_SRCS webexconfigbase.kcfgc)
kde4_add_kcfg_files(webexservice_SRCS webexcategory.kcfgc)


kde4_add_plugin(nepomukwebexservice ${webexservice_SRCS})


target_link_libraries(nepomukwebexservice 
    ${NEPOMUK_LIBRARIES}
    ${KDE4_NEPOMUK_LIBS}
    ${LIBWEBEXTRACTOR_LIBRARIES}
  )

kde4_add_executable( test_launch2 test_launch2.cpp ${webexservice_SRCS} )
target_link_libraries( test_launch2 
    ${LIBWEBEXTRACTOR_LIBRARIES}
    )

install(
  FILES nepomukwebexservice.desktop
  DESTINATION ${SERVICES_INSTALL_DIR})

#install(
#  FILES nepomukstrigiservice.notifyrc
#  DESTINATION ${DATA_INSTALL_DIR}/nepomukstrigiservice)

install(
  TARGETS nepomukwebexservice
  DESTINATION ${PLUGIN_INSTALL_DIR})

install(
    FILES webextractor.kcfg DESTINATION ${KCFG_INSTALL_DIR})
