set( decisions_SRCS 
    #Decisions part
    identsetmanager.cpp
    decision.cpp
    decisioncreator.cpp
    decisioncreator_p.cpp
    decisionfactory.cpp
    decisionlist.cpp
    decisionapplicationrequest.cpp
    propertiesgroup.cpp
    propertiesgroupcreator.cpp
    propertiesgroupcreator_p.cpp
    )
soprano_add_ontology( decisions_SRCS ${libwebextractor_SOURCE_DIR}/ontology/decisions.trig "NDCO" "Nepomuk::WebExtractor::Vocabulary" "trig" VISIBILITY WEBEXTRACTOR )

kde4_add_library( decisions STATIC ${decisions_SRCS} ) 


IF(UNIX AND NOT WIN32)
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
      SET_TARGET_PROPERTIES(decisions PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
ENDIF(UNIX AND NOT WIN32)
