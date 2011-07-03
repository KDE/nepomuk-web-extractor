set( rsd_SRCS
    # Resource service data part
   resourceservicedatamanager.cpp
   resourceservicedata.cpp
   nepomukservicedatabackend.cpp
   dummyservicedatabackend.cpp
   servicedatabackend.cpp
    )

soprano_add_ontology( rsd_SRCS ${nepomuk_webextractor_SOURCE_DIR}/ontology/decisions.trig "NDCO" "Nepomuk::WebExtractor::Vocabulary" "trig" VISIBILITY WEBEXTRACTOR )
kde4_add_library( rsd STATIC ${rsd_SRCS} )

IF(UNIX AND NOT WIN32)
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
      SET_TARGET_PROPERTIES(rsd PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
ENDIF(UNIX AND NOT WIN32)
