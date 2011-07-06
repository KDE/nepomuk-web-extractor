project(nepomuk_decision)


# Find necessary packges

#include directories

include_directories(
    ${nepomukgraph_SOURCE_DIR}
    )

set( decisions_SRCS 
    identsetmanager.cpp
    decision.cpp
    decisionmetadata.cpp
    decisioncreator.cpp
    decisioncreator_p.cpp
    decisionfactory.cpp
    decisionlist.cpp
    #decisionapplicationrequest.cpp
    propertiesgroup.cpp
    propertiesgroupcreator.cpp
    propertiesgroupcreator_p.cpp
    global.cpp
    )

set( decisions_HEADERS
    decisionfactory.h
    decision.h
    decisionmetadata.h
    #decisionapplicationrequest.h
    decisionlist.h
    decisioncreator.h
    propertiesgroup.h
    propertiesgroupcreator.h
    identsetmanager.h
    applyerror.h
    global.h
    )

set( LIBNEPOMUK_DECISION_VERSION "0.0.1" )
set ( LIB_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
# Runtime dependincies ( shared libraries )
set ( LIBNEPOMUK_DECISION_DEPENDENCIES
    ${NEPOMUK_LIBRARIES}
    ${NEPOMUK_QUERY_LIBRARIES}
    ${KDE4_NEPOMUK_LIBS}
    ${QT_LIBRARIES}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    nepomukgraph
    nepomukdatamanagement
    )

set ( LIBNEPOMUK_DECISION_LIBRARIES 
    # Our library
    nepomuk_decision
    # And all dependencies
    ${LIBNEPOMUK_DECISION_DEPENDENCIES}
    )

set(LIBNEPOMUK_DECISION_INCLUDES
    ${CMAKE_INSTALL_PREFIX}/include 
    ${CMAKE_INSTALL_PREFIX}/include/nepomuk 
    ${CMAKE_INSTALL_PREFIX}/include/nepomuk/decision
    )

configure_file(LibNepomukDecisionConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/LibNepomukDecisionConfig.cmake @ONLY )
#configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY )


kde4_add_library( nepomuk_decision SHARED ${decisions_SRCS} ) 

target_link_libraries( nepomuk_decision
    ${LIBNEPOMUK_DECISION_DEPENDENCIES}
    nepomuksynclegacy
  )

IF(UNIX AND NOT WIN32)
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
      SET_TARGET_PROPERTIES(nepomuk_decision PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
ENDIF(UNIX AND NOT WIN32)

install ( TARGETS nepomuk_decision
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
    )

install(FILES
    ${decisions_HEADERS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/nepomuk/decision/
    )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LibNepomukDecisionConfig.cmake
        DESTINATION ${LIB_DESTINATION})
