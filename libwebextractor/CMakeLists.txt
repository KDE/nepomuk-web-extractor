project(libwebextractor)

add_subdirectory(graph)
option( WEBEXTRACTOR_ENABLE_TESTS "Enable testing" OFF )

set(WEBEXTRACTOR_VERSION_MAJOR 0 CACHE INT "Major WebExtractor version number" FORCE)
set(WEBEXTRACTOR_VERSION_MINOR 0 CACHE INT "Minor WebExtractor version number" FORCE)
set(WEBEXTRACTOR_VERSION_PATCH 1 CACHE INT "Release WebExtractor version number" FORCE)
set(WEBEXTRACTOR_VERSION_STRING "${WEBEXTRACTOR_VERSION_MAJOR}.${WEBEXTRACTOR_VERSION_MINOR}.${WEBEXTRACTOR_VERSION_PATCH}" CACHE STRING "Webextractor version string" FORCE)
set( LIBWEBEXTRACTOR_VERSION $WEBEXTRACTOR_VERSION_STRING})

# definition of LIB_DESTINATION which is the path where the lib is installed
set (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
set (LIB_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING "Library directory name" FORCE)

configure_file(LibWebextractorConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/LibWebextractorConfig.cmake @ONLY )
configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY )

#build dependencies
add_subdirectory(ontologyloader)

include_directories(
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/decisions/
  ${CMAKE_CURRENT_SOURCE_DIR}/resourceservicedata/
  ${CMAKE_CURRENT_SOURCE_DIR}/graph/
  ${ontologyloader_SOURCE_DIR}
  #  ${CMAKE_CURRENT_SOURCE_DIR}/include
  #${CMAKE_CURRENT_SOURCE_DIR}/include/webextractor
  ${CMAKE_CURRENT_BINARY_DIR}
  ${NEPOMUK_INCLUDE_DIR}
  ${KDE4_INCLUDES}
  ${backupsynclegacy_SOURCE_DIR}
  )

set(backupsynclegacy_SRC
    ${backupsynclegacy_SOURCE_DIR}/changelog.cpp
    ${backupsynclegacy_SOURCE_DIR}/changelogrecord.cpp
    ${backupsynclegacy_SOURCE_DIR}/identificationrequest.cpp
    ${backupsynclegacy_SOURCE_DIR}/identificationrequest_p.cpp
    ${backupsynclegacy_SOURCE_DIR}/identificationset.cpp
    ${backupsynclegacy_SOURCE_DIR}/mergerequest.cpp
    ${backupsynclegacy_SOURCE_DIR}/syncfile.cpp
    ${backupsynclegacy_SOURCE_DIR}/simpleresource.cpp
    ${backupsynclegacy_SOURCE_DIR}/graphwrapper.cpp
)
soprano_add_ontology(backupsynclegacy_SRC
  ${backupsynclegacy_SOURCE_DIR}/identification.trig
  "backupsync"
  "Nepomuk::Vocabulary"
  "trig")

add_definitions(-DMAKE_WEBEXTRACTOR_LIB)
add_subdirectory(decisions)
add_subdirectory(resourceservicedata)
set( webextractor_SRCS 
    #Decisions part
    #    decisions/decision.cpp
    #    decisions/decisioncreator.cpp
    #    decisions/decisioncreator_p.cpp
    #    decisions/decisionfactory.cpp
    #    decisions/decisionlist.cpp
    #    decisions/decisionapplicationrequest.cpp
    #    decisions/propertiesgroup.cpp
    #    decisions/propertiesgroupcreator.cpp
    #    decisions/propertiesgroupcreator_p.cpp
    #Main part
    resourceanalyzerfactory.cpp
    resourceanalyzer.cpp
    executivereply.cpp
    executive.cpp
    executivewrapper.cpp
    debugexecutive.cpp
    parameters.cpp
    global.cpp
    simplenetworkexecutive.cpp
    simplenetworkreplyfactory.cpp
    simplenetworkreply.cpp
    simplenetworkrequest.cpp
    simpleexecutivereply.cpp
    # Resource service data part
    #    resourceservicedata/resourceservicedatamanager.cpp
    #    resourceservicedata/resourceservicedata.cpp
    #    resourceservicedata/nepomukservicedatabackend.cpp
    #    resourceservicedata/dummyservicedatabackend.cpp
    #    resourceservicedata/servicedatabackend.cpp
    #    identsetmanager.cpp
    )

set (WEBEXTRACTOR_HEADERS 
    # Decisions part
    #    decisions/decisionfactory.h
    #    decisions/decision.h
    #    decisions/decisionapplicationrequest.h
    #    decisions/decisionlist.h
    #    decisions/decisioncreator.h
    #    decisions/propertiesgroup.h
    #    decisions/propertiesgroupcreator.h
    # Main part
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
    identsetmanager.h
    ${CMAKE_CURRENT_BINARY_DIR}/ndco.h
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    ) 

soprano_add_ontology( webextractor_SRCS ${libwebextractor_SOURCE_DIR}/ontology/decisions.trig "NDCO" "Nepomuk::WebExtractor::Vocabulary" "trig" VISIBILITY WEBEXTRACTOR )


kde4_add_library( webextractor SHARED ${webextractor_SRCS})

set_target_properties(webextractor PROPERTIES
	VERSION ${WEBEXTRACTOR_VERSION_STRING}
	SOVERSION ${WEBEXTRACTOR_VERSION_MAJOR}
	DEFINE_SYMBOL MAKE_WEBEXTRACTOR_LIB
)


target_link_libraries( webextractor 
  ${NEPOMUK_LIBRARIES}
  ${NEPOMUK_QUERY_LIBRARIES}
  ${KDE4_NEPOMUK_LIBS}
  ${QT_LIBRARIES}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ontologyloader
  nepomukgraph
  decisions
  rsd
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
