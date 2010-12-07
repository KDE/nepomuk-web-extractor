project( sopranograph )
cmake_minimum_required(VERSION 2.6)

## ============ Find required packages ================
find_package(Soprano REQUIRED)

set(QT_MIN_VERSION "4.5.0")
find_package(Qt4 REQUIRED)
# properly set up compile flags (QT_DEBUG/QT_NO_DEBUG, ...)
include(${QT_USE_FILE})

## ========== Find optional packages =================
find_package(VTK)
if (VTK_FOUND)
    include(${VTK_USE_FILE})
endif (VTK_FOUND )

set(SOPRANOGRAPH_VERSION_MAJOR 0 CACHE INT "Major SopranoGraph version number" FORCE)
set(SOPRANOGRAPH_VERSION_MINOR 0 CACHE INT "Minor SopranoGraph version number" FORCE)
set(SOPRANOGRAPH_VERSION_PATCH 1 CACHE INT "Release SopranoGraph version number" FORCE)
set(SOPRANOGRAPH_VERSION_STRING "${SOPRANOGRAPH_VERSION_MAJOR}.${SOPRANOGRAPH_VERSION_MINOR}.${SOPRANOGRAPH_VERSION_PATCH}" CACHE STRING "Webextractor version string" FORCE)
set( LIBSOPRANOGRAPH_VERSION $SOPRANOGRAPH_VERSION_STRING})

# definition of LIB_DESTINATION which is the path where the lib is installed
set (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
set (LIB_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING "Library directory name" FORCE)


configure_file(LibSopranoGraphConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/LibSopranoGraphConfig.cmake @ONLY )
#configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY )

include_directories(
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

set (SG_SOURCES
    modelgraph.cpp
    graphalgorithm.cpp
    visitednodefilter.cpp
    dotvisitor.cpp
    nodefilterinterface.cpp
    selectedpropertiesfunc.cpp
    visitednodefilter.cpp
    resourcenodefilter.cpp
    childqueryinterface.cpp
    plaintextvisitor.cpp
    )

set ( SG_HEADERS
    graphalgorithm.h
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

## =========== Handle optional dependencies ============
if (VTK_FOUND )
    set (SG_SOURCES
        ${SG_SOURCES}
        vtkgraphvisitor.cpp
        )
    set (SG_HEADERS
        ${SG_HEADERS}
        vtkgraphvisitor.h
        )

    set (OPTIONAL_LIBS
        ${OPTIONAL_LIBS}
        vtkCommon
        vtkRendering
        )

endif(VTK_FOUND)


## ============== Add and install library =========
QT4_WRAP_CPP(SG_MOC ${SG_HEADERS})

add_library(sopranograph SHARED ${SG_SOURCES} ${SG_MOC})

set_target_properties(sopranograph PROPERTIES
	VERSION ${SOPRANOGRAPH_VERSION_STRING}
	SOVERSION ${SOPRANOGRAPH_VERSION_MAJOR}
	DEFINE_SYMBOL MAKE_SOPRANOGRAPH_LIB
)


target_link_libraries( sopranograph 
    #${NEPOMUK_LIBRARIES}
    #${NEPOMUK_QUERY_LIBRARIES}
    #${KDE4_NEPOMUK_LIBS}
  ${QT_LIBRARIES}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${SOPRANO_LIBRARIES}
  ${OPTIONAL_LIBS}
  )

install(TARGETS sopranograph
    	LIBRARY DESTINATION ${LIB_DESTINATION}
    	RUNTIME DESTINATION bin
    	ARCHIVE DESTINATION ${LIB_DESTINATION}
)
remove_definitions(-DMAKE_SOPRANOGRAPH_LIB)

install(FILES
	${SOPRANOGRAPH_HEADERS}
	DESTINATION include/sopranograph
	)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LibSopranoGraphConfig.cmake
	DESTINATION ${LIB_DESTINATION}/sopranograph)
