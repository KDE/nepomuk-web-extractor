project(libwebextractor)

cmake_minimum_required(VERSION 2.6)

set (QT_MIN_VERSION "4.6.0")
set (QT_USE_QTNETWORK true)


find_package(Qt4  REQUIRED)
find_package(KDE4  REQUIRED)
find_package(Nepomuk REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)


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
  )


add_definitions(-DMAKE_WEBEXTRACTOR_LIB)
set( webextractor_SRCS 
    resourceanalyzerfactory.cpp
    resourceanalyzer.cpp
    decision.cpp
    decisiondata.cpp
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
    plaintextvisitor.cpp
    )

set ( webextractor_MOC    
    resourceanalyzerfactory.h 
    resourceanalyzer.h
    datappreply.h
    decisionapplicationrequest.h
    datapp.h
    debug_datapp.h
    simplenetworkdatapp.h
    simplenetworkreply.h
    simpledatappreply.h
    simplenetworkreplyfactory.h
    simplenetworkrequest.h
    nepomukservicedatabackend.h
    dummyservicedatabackend.h
    servicedatabackend.h
    # Graph part
    visitednodefilter.h
    resourcenodefilter.h
    selectedpropertiesfunc.h
    childqueryinterface.h
    resourcenodefilter.h
    nodefilterinterface.h
    )

QT4_WRAP_CPP( webextractor_RES ${webextractor_MOC} )
set ( webextractor_SRCS
    ${webextractor_SRCS}
    ${webextractor_RES}
    )

soprano_add_ontology( webextractor_SRCS ${libwebextractor_SOURCE_DIR}/decisions.trig "NDCO" "Nepomuk::WebExtractor::Vocabulary" "trig" VISIBILITY WEBEXTRACTOR )
#nepomuk_add_ontology_classes( webextractor_SRCS, ONTOLOGIES ${libwebextractor_SOURCE_DIR}/decisions.trig CLASSES DataPP VISIBILITY webextractor)

add_library( webextractor SHARED ${webextractor_SRCS})

#kde4_add_library( webextractor SHARED ${webextractor_SRCS} )

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
  nepomuksync
  ontologyloader
  )

install(TARGETS webextractor
    	LIBRARY DESTINATION ${LIB_DESTINATION}
    	RUNTIME DESTINATION bin
    	ARCHIVE DESTINATION ${LIB_DESTINATION}
)
remove_definitions(-DMAKE_WEBEXTRACTOR_LIB)



#installing

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
    decisiondata.h
    global.h
    parameters.h
    propertiesgroup.h
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
#set ( WEBEXTRACTOR_HEADERS 
#    ${CMAKE_CURRENT_SOURCE_DIR}/include/webextractor/resourceanalyzerfactory.h 
#    ${CMAKE_CURRENT_SOURCE_DIR}/include/webextractor/resourceanalyzer.h
#    )

install(FILES
	${WEBEXTRACTOR_HEADERS}
	DESTINATION include/webextractor
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
