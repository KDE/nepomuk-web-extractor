project(nepomukgraph)

set(NEPOMUKGRAPH_VERSION_MAJOR 0 CACHE INT "Major NepomukGraph version number" FORCE)
set(NEPOMUKGRAPH_VERSION_MINOR 0 CACHE INT "Minor NepomukGraph version number" FORCE)
set(NEPOMUKGRAPH_VERSION_PATCH 1 CACHE INT "Release NepomukGraph version number" FORCE)
set(NEPOMUKGRAPH_VERSION_STRING "${NEPOMUKGRAPH_VERSION_MAJOR}.${NEPOMUKGRAPH_VERSION_MINOR}.${NEPOMUKGRAPH_VERSION_PATCH}" CACHE STRING "NepomukGraph version string" FORCE)

set( LIB_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" CACHE STRING "Library directory name" FORCE )

include_directories(
  ${KDE4_INCLUDES}
  )

add_definitions(-DMAKE_NEPOMUKGRAPH_LIB)

configure_file(LibNepomukgraphConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/LibNepomukgraphConfig.cmake @ONLY )

set( nepomukgraph_SRCS 
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
    )

set( nepomukgraph_HEADERS
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

kde4_add_library( nepomukgraph SHARED ${nepomukgraph_SRCS})

target_link_libraries( nepomukgraph 
  ${NEPOMUK_LIBRARIES}
  ${NEPOMUK_QUERY_LIBRARIES}
  ${KDE4_NEPOMUK_LIBS}
  )

install(TARGETS nepomukgraph
    	LIBRARY DESTINATION ${LIB_DESTINATION}
    	RUNTIME DESTINATION bin
    	ARCHIVE DESTINATION ${LIB_DESTINATION}
)
remove_definitions(-DMAKE_NEPOMUKGRAPH_LIB)

install(FILES
    ${nepomukgraph_HEADERS}
	DESTINATION ${INCLUDE_INSTALL_DIR}/nepomuk
    COMPONENT Devel
	)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LibNepomukgraphConfig.cmake
	DESTINATION ${LIB_DESTINATION})
