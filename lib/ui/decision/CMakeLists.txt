project(nepomuk_decisionui)

set(DECISIONUI_VERSION_MAJOR 0 CACHE INT "Major Decision UI version number" FORCE)
set(DECISIONUI_VERSION_MINOR 0 CACHE INT "Minor Decision UI version number" FORCE)
set(DECISIONUI_VERSION_PATCH 1 CACHE INT "Release Decision UI version number" FORCE)
set(DECISIONUI_VERSION_STRING "${WEBEXTRACTOR_VERSION_MAJOR}.${WEBEXTRACTOR_VERSION_MINOR}.${WEBEXTRACTOR_VERSION_PATCH}" CACHE STRING "Decision ui version string" FORCE)
set( LIBDECISIONUI_VERSION $DECISIONUI_VERSION_STRING})

# definition of LIB_DESTINATION which is the path where the lib is installed
set (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
set (LIB_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING "Library directory name" FORCE)

set( LIBDECISIONUI_DEPENDENCIES
    ${SOPRANO_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KDEUI_LIBRARY}
    nepomuk_decision
    nepomukdatamanagement
  )

set( LIBDECISIONUI_LIBRARIES
    ${LIBDECISIONUI_DEPENDENCIES}
    nepomuk_decisionui
    )

set( LIBDECISIONUI_INCLUDE_DIRS
  ${LIBNEPOMUK_DECISION_INCLUDE_DIRS}
  ${NEPOMUK_INCLUDE_DIR}
  ${CMAKE_INSTALL_PREFIX}/include/nepomuk/decision
  )


configure_file(LibDecisionuiConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/LibDecisionuiConfig.cmake @ONLY )

include_directories(
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${backupsynclegacy_SOURCE_DIR}
  )

add_definitions(-DMAKE_DECISIONUI_LIB)
set( decisionui_SRCS 
    changelogwidget.cpp
    decisionwidget.cpp
    decisioncollectionwidget.cpp
    )

set (decisionui_HEADERS 
    decisionui_export.h
    changelogwidget.h
    decisionwidget.h
    decisioncollectionwidget.h
    ) 

set( decisionui_UI
    changeLogForm.ui
    decisionForm.ui
    )


kde4_add_ui_files( decisionui_SRCS
   ${decisionui_UI}
  )


kde4_add_library( nepomuk_decisionui SHARED ${decisionui_SRCS} ${backupsynclegacy_SRC})


set_target_properties(nepomuk_decisionui PROPERTIES
        VERSION ${WEBEXTRACTORUI_VERSION_STRING}
        SOVERSION ${WEBEXTRACTORUI_VERSION_MAJOR}
        DEFINE_SYMBOL MAKE_DECISIONUI_LIB
        )

remove_definitions(-DMAKE_DECISIONUI_LIB)
target_link_libraries( nepomuk_decisionui 
    ${LIBDECISIONUI_DEPENDENCIES}
  )

install(TARGETS nepomuk_decisionui
        LIBRARY DESTINATION ${LIB_DESTINATION}
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION ${LIB_DESTINATION}
        )
remove_definitions(-DMAKE_WEBEXTRACTORUI_LIB)


#Qt designer plugins
set (decisionwidgetdesignerplugin_SRCS
    decisionwidgetdesignerplugin.cpp
    )
kde4_add_plugin( decisionwidgetdesignerplugin 
    ${decisionwidgetdesignerplugin_SRCS}
    )

target_link_libraries( decisionwidgetdesignerplugin
        ${QT_LIBRARIES}
        ${KDE4_KIO_LIBS}
        nepomuk_decisionui
        )

#installing

#set ( WEBEXTRACTOR_HEADERS 
#    ${CMAKE_CURRENT_SOURCE_DIR}/include/webextractor/resourceanalyzerfactory.h 
#    ${CMAKE_CURRENT_SOURCE_DIR}/include/webextractor/resourceanalyzer.h
#    )

install(FILES
        ${decisionui_HEADERS}
        DESTINATION ${INCLUDE_INSTALL_DIR}/nepomuk/decision
        )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LibDecisionuiConfig.cmake
        DESTINATION ${LIB_DESTINATION}
        )


install( TARGETS decisionwidgetdesignerplugin  DESTINATION ${PLUGIN_INSTALL_DIR}/plugins/designer )
