add_definitions(-DMAKE_WEBEXTRACTOR_LIB)
set( webextractor_SRCS 
    resourceanalyzerfactory.cpp
    resourceanalyzer.cpp
    )

#set ( webextractor_MOC    webextractor_settings.h    )
set ( webextractor_MOC    
    ${libwebextractor_SOURCE_DIR}/include/webextractor/resourceanalyzerfactory.h 
    ${libwebextractor_SOURCE_DIR}/include/webextractor/resourceanalyzer.h
    resourceanalyzerimpl.h
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


target_link_libraries( webextractor 
  ${NEPOMUK_LIBRARIES}
  ${KDE4_NEPOMUK_LIBS}
  )

install(TARGETS webextractor
    	LIBRARY DESTINATION ${LIB_DESTINATION}
    	RUNTIME DESTINATION bin
    	ARCHIVE DESTINATION ${LIB_DESTINATION}
)
remove_definitions(-DMAKE_WEBEXTRACTOR_LIB)
