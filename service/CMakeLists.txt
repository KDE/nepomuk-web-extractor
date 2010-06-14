project(webextractorservice)

cmake_minimum_required(VERSION 2.6)

set (QT_MIN_VERSION "4.6.0")
set (QT_USE_QTNETWORK true)

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

set( webexplugin_SRCS
        webextractor_plugin.cpp
    )

FILE( GLOB 
    webexservice_categories_CFG
    config/categories/*rc
    )


qt4_add_dbus_adaptor(webexservice_SRCS  org.kde.nepomuk.WebExtractorService.xml webexservice.h Nepomuk::WebExtractorService )





kde4_add_library ( webexplugin SHARED 
    ${webexplugin_SRCS} 
    )

# Add webexsettings library, that depends from webexplugin
add_subdirectory(libwebexsettings)

# Add include directory for libwebexsettings library
include_directories(
    ${libwebexsettings_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/libwebexsettings
    )


#building

target_link_libraries( webexplugin
    ${LIBWEBEXTRACTOR_LIBRARIES}
    ${QT_CORE_LIBRARY}
   )

#service
kde4_add_plugin(nepomukwebexservice ${webexservice_SRCS})

#kcm
add_subdirectory(kcm)


target_link_libraries(nepomukwebexservice 
    ${NEPOMUK_LIBRARIES}
    ${KDE4_NEPOMUK_LIBS}
    ${LIBWEBEXTRACTOR_LIBRARIES}
    webexsettings
  )

kde4_add_executable( test_launch2 test_launch2.cpp ${webexservice_SRCS} )

kde4_add_executable( test_config test_config.cpp ${webexservice_SRCS} )

#include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/plugins/debug )
#QT4_WRAP_CPP( test_config_SRCS test_simple.cpp )
#kde4_add_executable( test_simple test_simple.cpp ${CMAKE_CURRENT_SOURCE_DIR}/plugins/debug/simple_debug_reply.cpp  )

#target_link_libraries( test_simple
#    ${LIBWEBEXTRACTOR_LIBRARIES}
#    )


target_link_libraries( test_launch2 
    ${LIBWEBEXTRACTOR_LIBRARIES}
    webexsettings
    )

target_link_libraries( test_config 
    ${LIBWEBEXTRACTOR_LIBRARIES}
    webexsettings
    )


add_subdirectory(plugins)

install(
  FILES nepomukwebexservice.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
  )

install(
  FILES webextractor_plugin.desktop
  DESTINATION ${SERVICETYPES_INSTALL_DIR}
  )

#install(
#  FILES webextractordatapp.desktop
#  DESTINATION ${SERVICETYPES_INSTALL_DIR}
#  )
#install(
#  FILES nepomukstrigiservice.notifyrc
#  DESTINATION ${DATA_INSTALL_DIR}/nepomukstrigiservice)

install(TARGETS webexplugin
    	LIBRARY DESTINATION ${LIB_DESTINATION}
    	RUNTIME DESTINATION bin
    	ARCHIVE DESTINATION ${LIB_DESTINATION}
)
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


# install headers
install (
    FILES webextractor_plugin.h DESTINATION include/webextractor
    )
