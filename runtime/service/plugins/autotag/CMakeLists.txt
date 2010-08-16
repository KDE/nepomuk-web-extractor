project(webexplugin_autotag)

cmake_minimum_required(VERSION 2.6)

set (QT_MIN_VERSION "4.6.0")
set (QT_USE_QTNETWORK true)


find_package(KDE4  REQUIRED)
find_package(Nepomuk REQUIRED)

# check for required packages
if(WEBEXTRACTOR_VERSION_STRING)
	# if WEBEXTRACTOR_VERSION_STRING is defined, we are compiling the meta-package
	set(LIBWEBEXTRACTOR_INCLUDE_DIRS
		../libwebextractor
		${CMAKE_CURRENT_BINARY_DIR}/../libwebextractor)
	set(LIBWEBEXTRACTOR_LIBRARIES webextractor)
else(WEBEXTRACTOR_VERSION_STRING)
	find_package(LibWebextractor REQUIRED)
endif(WEBEXTRACTOR_VERSION_STRING)


if(WEBEXTRACTOR_SERVICE_VERSION_STRING)
    # if WEBEXTRACTOR_SERVICE_VERSION_STRING is defined then plugins are compiled together with service
    set( WEBEXSERVICE_INCLUDE_DIRS
	webextractorservice_SOURCE_DIR
	webextractorservice_BINARY_DIR
    )	
endif(WEBEXTRACTOR_SERVICE_VERSION_STRING)

include_directories(
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${NEPOMUK_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${LIBWEBEXTRACTOR_INCLUDE_DIRS}
  ${WEBEXSERVICE_INCLUDE_DIRS}
  )


link_directories(${LIBWEBEXTRACTOR_LIBRARY_DIRS})

set (autotag_plugin_SRCS
    autotagplugin.cpp
    autotagdatappreply.cpp
    autotagdatapp.cpp
    )

set (AUTOTAG_PLUGIN_VERSION 0.1)

configure_file(autotagplugin_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/autotagplugin_config.h  )

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    )

kde4_add_plugin(webexplugin_autotag ${autotag_plugin_SRCS})

target_link_libraries( webexplugin_autotag
    ${LIBWEBEXTRACTOR_LIBRARIES}
    webexplugin
    ${NEPOMUK_LIBRARIES}
    ${KDE4_NEPOMUK_LIBS}
    ${QT_LIBRARIES}
    nepomukutils
    )

install(
  TARGETS webexplugin_autotag
  DESTINATION ${PLUGIN_INSTALL_DIR}
  )

install(
  FILES webexplugin_autotag.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
  )


