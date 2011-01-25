project(libnepomuksynclegacy)

find_package(KDE4 REQUIRED)
find_package(Nepomuk REQUIRED)

include(KDE4Defaults)
include(SopranoAddOntology)

include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
  ${KDE4_KDECORE_INCLUDES}
  ${SOPRANO_INCLUDE_DIR}
  ${NEPOMUK_INCLUDE_DIR}
)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions(-DDISABLE_NEPOMUK_LEGACY=1)

set(nepomuksynclegacy_SRCS
    changelog.cpp
    changelogrecord.cpp
    identificationset.cpp
    syncfile.cpp
    identificationrequest.cpp
    identificationrequest_p.cpp
    mergerequest.cpp
    simpleresource.cpp
    graphwrapper.cpp
)

soprano_add_ontology(nepomuksynclegacy_SRCS
   ${CMAKE_CURRENT_SOURCE_DIR}/identification.trig
  "backupsync"
  "Nepomuk::Vocabulary"
  "trig")

kde4_add_library(nepomuksynclegacy SHARED ${nepomuksynclegacy_SRCS})

set_target_properties(nepomuksynclegacy PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION})

target_link_libraries(nepomuksynclegacy
  ${QT_QTCORE_LIBRARY}
  ${SOPRANO_LIBRARIES}
  ${NEPOMUK_LIBRARIES}
  ${NEPOMUK_QUERY_LIBRARIES}
  ${KDE4_KIO_LIBS}
  kdecore
  )

#install(TARGETS nepomuksynclegacy EXPORT kdelibsLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})
#
#install(FILES
#    syncfile.h
#    changelog.h
#    changelogrecord.h
#    identificationset.h
#    identificationrequest.h
#    mergerequest.h
#    simpleresource.h
#    graphwrapper_p.h
#    misc.h
#    nepomuksync_export.h
#    DESTINATION ${INCLUDE_INSTALL_DIR}/nepomuk COMPONENT Devel
#)
