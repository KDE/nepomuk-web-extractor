project(libwebextractor)

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
set( webextractor_SRCS 
    resourceanalyzerfactory.cpp
    resourceanalyzer.cpp
    decision.cpp
    decisioncreator.cpp
    decisioncreator_p.cpp
    decisionfactory.cpp
    decisionlist.cpp
    decisionapplicationrequest.cpp
    datappreply.cpp
    datapp.cpp
    debug_datapp.cpp
    parameters.cpp
    global.cpp
    datappwrapper.cpp
    propertiesgroup.cpp
    propertiesgroupcreator.cpp
    propertiesgroupcreator_p.cpp
    simplenetworkdatapp.cpp
    simplenetworkreplyfactory.cpp
    simplenetworkreply.cpp
    simplenetworkrequest.cpp
    simpledatappreply.cpp
    resourceservicedatamanager.cpp
    resourceservicedata.cpp
    nepomukservicedatabackend.cpp
    dummyservicedatabackend.cpp
    servicedatabackend.cpp
    identsetmanager.cpp
    # Graph  part
    modelgraph.cpp
    algorithm.cpp
    graphalgorithm.cpp
    copyvisitor.cpp
    visitednodefilter.cpp
    dotvisitor.cpp
    nodefilterinterface.cpp
    selectedpropertiesfunc.cpp
    visitednodefilter.cpp
    resourcenodefilter.cpp
    childqueryinterface.cpp
    plaintextvisitor.cpp

    ${backupsynclegacy_SRC}
    )

set (WEBEXTRACTOR_HEADERS 
    datapp.h
    datappkeeper.h
    datappreply.h
    datappreply_p.h
    datapprequest.h
    datappwrapper.h
    datapptemplate.h
    debug_datapp.h
    decisionfactory.h
    decision.h
    decisionapplicationrequest.h
    decisionlist.h
    decisioncreator.h
    global.h
    parameters.h
    propertiesgroup.h
    propertiesgroupcreator.h
    resourceanalyzerfactory.h
    resourceanalyzer.h
    simpledatappreply.h
    simplenetworkdatapp.h
    simplenetworkreplyfactory.h
    simplenetworkreply.h
    simplenetworkrequest.h
    resourceservicedatamanager.h
    resourceservicedata.h
    nepomukservicedatabackend.h
    dummyservicedatabackend.h
    nepomukservicedatabackendfactory.h
    servicedatabackend.h
    servicedatabackendfactory.h
    webextractor_export.h
    identsetmanager.h
    algorithm.h
    modelgraphvisitor.h
    ${CMAKE_CURRENT_BINARY_DIR}/ndco.h
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    # Graph  part
    algorithm.h
    graphalgorithm.h
    copyvisitor.h
    childqueryinterface.h
    visitednodefilter.h
    nodefilterinterface.h
    resourcenodefilter.h
    dotvisitor.h
    modelgraphvisitor.h
    modelgraph.h
    selectedpropertiesfunc.h
    plaintextvisitor.h
    ) 

soprano_add_ontology( webextractor_SRCS ${libwebextractor_SOURCE_DIR}/decisions.trig "NDCO" "Nepomuk::WebExtractor::Vocabulary" "trig" VISIBILITY WEBEXTRACTOR )

#QT4_WRAP_CPP( webextractor_RES ${webextractor_MOC} )
#set ( webextractor_SRCS
#    ${webextractor_SRCS}
#    ${webextractor_RES}
#    )


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

install_ontology(decisions nepomuk)

## TESTS
if ( WEBEXTRACTOR_ENABLE_TESTS)
    add_subdirectory(test)
endif(WEBEXTRACTOR_ENABLE_TESTS )
