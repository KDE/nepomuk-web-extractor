add_definitions(-DMAKE_WEBEXTRACTOR_LIB)
set( webextractor_SRCS 
    resourceanalyzerfactory.cpp
    resourceanalyzer.cpp
    decision.cpp
    decisionfactory.cpp
    decisionlist.cpp
    datappreply.cpp
    datapp.cpp
    debug_datapp.cpp
    parameters.cpp
    global.cpp
    datappwrapper.cpp
    #simple_datapp.cpp
    #simple_reply_factory.cpp
    #simple_reply.cpp
    )

#set ( webextractor_MOC    webextractor_settings.h    )
set ( webextractor_MOC    
    ${libwebextractor_SOURCE_DIR}/include/webextractor/resourceanalyzerfactory.h 
    ${libwebextractor_SOURCE_DIR}/include/webextractor/resourceanalyzer.h
    ${libwebextractor_SOURCE_DIR}/include/webextractor/datappreply.h
    ${libwebextractor_SOURCE_DIR}/include/webextractor/debug_datapp.h
    #${libwebextractor_SOURCE_DIR}/include/webextractor/simple_datapp.h
    #${libwebextractor_SOURCE_DIR}/include/webextractor/simple_reply.h
    #${libwebextractor_SOURCE_DIR}/include/webextractor/simple_reply_factory.h
    #    resourceanalyzerimpl.h
    )

QT4_WRAP_CPP( webextractor_RES ${webextractor_MOC} )
set ( webextractor_SRCS
    ${webextractor_SRCS}
    ${webextractor_RES}
    )

add_library( webextractor SHARED ${webextractor_SRCS})

#kde4_add_library( webextractor SHARED ${webextractor_SRCS} )

set_target_properties(webextractor PROPERTIES
	VERSION ${WEBEXTRACTOR_VERSION_STRING}
	SOVERSION ${WEBEXTRACTOR_VERSION_MAJOR}
	DEFINE_SYMBOL MAKE_WEBEXTRACTOR_LIB
)

#message(STATUS "(NOT) Found Qt network library ${QT_LIBRARIES} ")

target_link_libraries( webextractor 
  ${NEPOMUK_LIBRARIES}
  ${KDE4_NEPOMUK_LIBS}
  ${QT_LIBRARIES}
  ${QT_QTNETWORK_LIBRARY}
  )

install(TARGETS webextractor
    	LIBRARY DESTINATION ${LIB_DESTINATION}
    	RUNTIME DESTINATION bin
    	ARCHIVE DESTINATION ${LIB_DESTINATION}
)
remove_definitions(-DMAKE_WEBEXTRACTOR_LIB)
