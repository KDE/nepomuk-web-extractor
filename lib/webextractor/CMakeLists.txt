project(nepomuk_webextractor)

option( WEBEXTRACTOR_ENABLE_TESTS "Enable testing" OFF )

    
set(WEBEXTRACTOR_VERSION_MAJOR 0 CACHE INT "Major WebExtractor version number" FORCE)
set(WEBEXTRACTOR_VERSION_MINOR 0 CACHE INT "Minor WebExtractor version number" FORCE)
set(WEBEXTRACTOR_VERSION_PATCH 1 CACHE INT "Release WebExtractor version number" FORCE)
set(WEBEXTRACTOR_VERSION_STRING "${WEBEXTRACTOR_VERSION_MAJOR}.${WEBEXTRACTOR_VERSION_MINOR}.${WEBEXTRACTOR_VERSION_PATCH}" CACHE STRING "Webextractor version string" FORCE)
set( LIBWEBEXTRACTOR_VERSION $WEBEXTRACTOR_VERSION_STRING})

# definition of LIB_DESTINATION which is the path where the lib is installed
set (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
set (LIB_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING "Library directory name" FORCE)

set ( LIBWEBEXTRACTOR_DEPENDENCIES
    ${NEPOMUK_LIBRARIES}
    ${NEPOMUK_QUERY_LIBRARIES}
    ${KDE4_NEPOMUK_LIBS}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    nepomuk_decision
    )

set( LIBWEBEXTRACTOR_LIBRARIES
    libwebextractor
    ${LIBWEBEXTRACTOR_DEPENDENCIES}
    )

set (LIBWEBEXTRACTOR_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/resourceservicedata/
  ${CMAKE_CURRENT_BINARY_DIR}
  )

set(LIBWEBEXTRACTOR_INCLUDE_DIRS 
    ${CMAKE_INSTALL_PREFIX}/include 
    ${CMAKE_INSTALL_PREFIX}/include/webextractor
    )

configure_file(LibWebextractorConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/LibWebextractorConfig.cmake @ONLY )
configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY )

include_directories(
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/decisions/
  ${CMAKE_CURRENT_SOURCE_DIR}/resourceservicedata/
  ${CMAKE_CURRENT_SOURCE_DIR}/graph/
  ${CMAKE_CURRENT_BINARY_DIR}
  ${ontologyloader_SOURCE_DIR}
  #  ${CMAKE_CURRENT_SOURCE_DIR}/include
  #${CMAKE_CURRENT_SOURCE_DIR}/include/webextractor
  ${CMAKE_CURRENT_BINARY_DIR}
  ${NEPOMUK_INCLUDE_DIR}
  ${KDE4_INCLUDES}
  ${backupsynclegacy_SOURCE_DIR}
  ${LIBNEPOMUK_DECISION_INCLUDE_DIRS}
  )

add_definitions(-DMAKE_WEBEXTRACTOR_LIB)
set( webextractor_SRCS 
    #Main part
    resourceanalyzerfactory.cpp
    resourceanalyzer.cpp
    executivereply.cpp
    executive.cpp
    executivewrapper.cpp
    debugexecutive.cpp
    parameters.cpp
    global.cpp
    # SimpleNetwork executive subsystem
    # doesn't work properly
    #simplenetworkexecutive.cpp
    #simplenetworkreplyfactory.cpp
    #simplenetworkreply.cpp
    #simplenetworkrequest.cpp
    simpleexecutivereply.cpp
    # Resource service data part
    #    resourceservicedata/resourceservicedatamanager.cpp
    #    resourceservicedata/resourceservicedata.cpp
    #    resourceservicedata/nepomukservicedatabackend.cpp
    #    resourceservicedata/dummyservicedatabackend.cpp
    #    resourceservicedata/servicedatabackend.cpp
    )

set (WEBEXTRACTOR_HEADERS 
    # Main part
    executive.h
    executivereply.h
    executivereply_p.h
    executivewrapper.h
    debugexecutive.h
    global.h
    parameters.h
    resourceanalyzerfactory.h
    resourceanalyzer.h
    simpleexecutivereply.h
    simplenetworkexecutive.h
    simplenetworkreplyfactory.h
    simplenetworkreply.h
    simplenetworkrequest.h
    webextractor_export.h
    # Resource service data part
    resourceservicedata/resourceservicedatamanager.h
    resourceservicedata/resourceservicedata.h
    resourceservicedata/nepomukservicedatabackend.h
    resourceservicedata/dummyservicedatabackend.h
    resourceservicedata/nepomukservicedatabackendfactory.h
    resourceservicedata/servicedatabackend.h
    resourceservicedata/servicedatabackendfactory.h
    ${CMAKE_CURRENT_BINARY_DIR}/ndco.h
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    ) 

soprano_add_ontology( webextractor_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/ontology/decisions.trig "NDCO" "Nepomuk::WebExtractor::Vocabulary" "trig" VISIBILITY WEBEXTRACTOR )


kde4_add_library( webextractor SHARED ${webextractor_SRCS})

set_target_properties(webextractor PROPERTIES
	VERSION ${WEBEXTRACTOR_VERSION_STRING}
	SOVERSION ${WEBEXTRACTOR_VERSION_MAJOR}
	DEFINE_SYMBOL MAKE_WEBEXTRACTOR_LIB
)

add_subdirectory(resourceservicedata)

target_link_libraries( webextractor 
    ${LIBWEBEXTRACTOR_DEPENDENCIES}
    rsd
    nepomuksynclegacy
    )

install(TARGETS webextractor
    	LIBRARY DESTINATION ${LIB_DESTINATION}
    	RUNTIME DESTINATION bin
    	ARCHIVE DESTINATION ${LIB_DESTINATION}
)
remove_definitions(-DMAKE_WEBEXTRACTOR_LIB)


install(FILES
	${WEBEXTRACTOR_HEADERS}
	DESTINATION ${INCLUDE_INSTALL_DIR}/webextractor
	)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LibWebextractorConfig.cmake
	DESTINATION ${LIB_DESTINATION}/libwebextractor)

macro(INSTALL_ONTOLOGY _name _group)
  set(ONTO_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/ontology/${_group})
  configure_file("${_name}.ontology.in" "${CMAKE_CURRENT_BINARY_DIR}/${_name}.ontology")
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${_name}.ontology" DESTINATION ${ONTO_INSTALL_DIR})
  install(FILES "${_name}.trig" DESTINATION ${ONTO_INSTALL_DIR})
endmacro(INSTALL_ONTOLOGY)

install_ontology(ontology/decisions nepomuk)

## TESTS
if ( WEBEXTRACTOR_ENABLE_TESTS)
    add_subdirectory(test)
endif(WEBEXTRACTOR_ENABLE_TESTS )
