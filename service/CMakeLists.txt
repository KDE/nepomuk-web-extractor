project(ddmsservice)


# check for required packages
#if(BUILD_METAPACKAGE)
#	# if WEBEXTRACTOR_VERSION_STRING is defined, we are compiling the meta-package
#    #set(LIBWEBEXTRACTOR_INCLUDE_DIRS
#    #	${libwebextractor_SOURCE_DIR})
#	set(LIBWEBEXTRACTOR_LIBRARIES webextractor)
#    message( STATUS "Building in metapackage. ")
#else(BUILD_METAPACKAGE)
#	find_package(LibWebextractor REQUIRED)
#    message( STATUS "Building in separate way " )
#endif(BUILD_METAPACKAGE)

set(DDM_SERVICE_VERSION_MAJOR 0 CACHE INT "Major Decision Management Service version number" FORCE)
set(DDM_SERVICE_VERSION_MINOR 0 CACHE INT "Minor Decision Management Service version number" FORCE)
set(DDM_SERVICE_VERSION_PATCH 1 CACHE INT "Release Decision Management Service version number" FORCE)
set(DDM_SERVICE_VERSION_STRING "${DDM_SERVICE_VERSION_MAJOR}.${DDM_SERVICE_VERSION_MINOR}.${DDM_SERVICE_VERSION_PATCH}" CACHE STRING "Decision Management version string" FORCE)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${NEPOMUK_INCLUDE_DIR}
  ${NEPOMUK_INCLUDE_DIR}/nepomuk
  )

set( ddm_service_SRCS
    ddms_core.cpp
    ddms_service.cpp
    ../common/decisionmanagementserviceadaptor.cpp
    )

## Adaptor can not be generated automatically
## because custom types are used in dbus interface

#qt4_add_dbus_adaptor(ddm_service_SRCS  
#    org.kde.nepomuk.DecisionManagementService.xml 
#    ddms_service.h 
#    Nepomuk::DecisionManagementService )



kde4_add_plugin(nepomukddmservice ${ddm_service_SRCS})

target_link_libraries(nepomukddmservice 
    ${NEPOMUK_LIBRARIES}
    ${KDE4_NEPOMUK_LIBS}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTSQL_LIBRARY}
    ${NEPOMUK_QUERY_LIBRARIES}
    ddms_common
  )

install(
  FILES nepomukddmservice.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
  )

install(
  TARGETS nepomukddmservice
  DESTINATION ${PLUGIN_INSTALL_DIR}
  )

