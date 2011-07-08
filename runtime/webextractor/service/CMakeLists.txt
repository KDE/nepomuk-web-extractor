project(webextractorservice)

# check for required packages
if(BUILD_METAPACKAGE)
	# if WEBEXTRACTOR_VERSION_STRING is defined, we are compiling the meta-package
    #set(LIBWEBEXTRACTOR_INCLUDE_DIRS
    #	${libwebextractor_SOURCE_DIR})
	set(LIBWEBEXTRACTOR_LIBRARIES webextractor)
    message( STATUS "Building in metapackage. ")
else(BUILD_METAPACKAGE)
	find_package(LibWebextractor REQUIRED)
    message( STATUS "Building in separate way " )
endif(BUILD_METAPACKAGE)

set(WEBEXTRACTOR_SERVICE_VERSION_MAJOR 0 CACHE INT "Major WebExtractor Service version number" FORCE)
set(WEBEXTRACTOR_SERVICE_VERSION_MINOR 0 CACHE INT "Minor WebExtractor Service version number" FORCE)
set(WEBEXTRACTOR_SERVICE_VERSION_PATCH 1 CACHE INT "Release WebExtractor Service version number" FORCE)
set(WEBEXTRACTOR_SERVICE_VERSION_STRING "${WEBEXTRACTOR_SERVICE_VERSION_MAJOR}.${WEBEXTRACTOR_SERVICE_VERSION_MINOR}.${WEBEXTRACTOR_SERVICE_VERSION_PATCH}" CACHE STRING "WebExtractor version string" FORCE)

include_directories(
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${NEPOMUK_INCLUDE_DIR}
  ${NEPOMUK_INCLUDE_DIR}/nepomuk
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${LIBWEBEXTRACTOR_INCLUDE_DIRS}
# Add include directory for libwebexsettings library
  ${libwebexsettings_BINARY_DIR}
  ${libwebexsettings_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../libwebexsettings
  # some files in service use #include "libwebexsettings/XXX"
  ${CMAKE_CURRENT_SOURCE_DIR}/../libwebexsettings/../
  )

set( webexservice_SRCS 
    webexservice.cpp
    webexcatscheduler.cpp
    webexscheduler.cpp
    webexqueries.cpp
    #webexcatschedulerimpl.cpp
    priority.cpp
    useractivitymonitor.cpp
    webextractor_settings.cpp
    #${webexsettings_SRCS}
    #webextractor_plugin.cpp
    )

FILE( GLOB 
    webexservice_categories_CFG
    config/categories/*rc
    )

FILE( GLOB 
    webexservice_datapps_CFG
    config/plugins/*rc
    )

qt4_add_dbus_adaptor(webexservice_SRCS  org.kde.nepomuk.WebExtractorService.xml webexservice.h Nepomuk::WebExtractorService )









#building

#service
kde4_add_plugin(nepomukwebexservice ${webexservice_SRCS})




target_link_libraries(nepomukwebexservice 
    ${NEPOMUK_LIBRARIES}
    ${KDE4_NEPOMUK_LIBS}
    ${KDE4_KDECORE_LIBS}
    ${LIBWEBEXTRACTOR_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${NEPOMUK_QUERY_LIBRARIES}
    webexsettings
  )

#kde4_add_executable( test_launch2 test_launch2.cpp ${webexservice_SRCS} )

#kde4_add_executable( test_config test_config.cpp ${webexservice_SRCS} )

#include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/plugins/debug )
#QT4_WRAP_CPP( test_config_SRCS test_simple.cpp )
#kde4_add_executable( test_simple test_simple.cpp ${CMAKE_CURRENT_SOURCE_DIR}/plugins/debug/simple_debug_reply.cpp  )

#target_link_libraries( test_simple
#    ${LIBWEBEXTRACTOR_LIBRARIES}
#    )


#target_link_libraries( test_launch2 
#    ${LIBWEBEXTRACTOR_LIBRARIES}
#    webexsettings
#    )
#
#target_link_libraries( test_config 
#    ${LIBWEBEXTRACTOR_LIBRARIES}
#    webexsettings
#    )



install(
  FILES nepomukwebexservice.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
  )


#install(
#  FILES webextractordatapp.desktop
#  DESTINATION ${SERVICETYPES_INSTALL_DIR}
#  )
#install(
#  FILES nepomukstrigiservice.notifyrc
#  DESTINATION ${DATA_INSTALL_DIR}/nepomukstrigiservice)

install(
  TARGETS nepomukwebexservice
  DESTINATION ${PLUGIN_INSTALL_DIR}
  )


install (
    FILES config/webextractorrc DESTINATION ${CONFIG_INSTALL_DIR}/webextractor 
    )

install (
    FILES ${webexservice_categories_CFG} DESTINATION ${CONFIG_INSTALL_DIR}/webextractor/categories 
    )


install (
    FILES ${webexservice_datapps_CFG} DESTINATION ${CONFIG_INSTALL_DIR}/webextractor/plugins
    )

# install headers
