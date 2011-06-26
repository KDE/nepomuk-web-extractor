set( ddms_client_SRCS
    client.cpp
    respond_proxies.cpp
    ../common/decisionmanagementserviceproxy.cpp
    )

set ( ddms_client_HEADERS
    client.h
    )

kde4_add_library(ddms_client SHARED
    ${ddms_client_SRCS}
    )

target_link_libraries( ddms_client
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ddms_common
    )

install( TARGETS ddms_client
    	LIBRARY DESTINATION lib
    	RUNTIME DESTINATION bin
    	ARCHIVE DESTINATION lib
	)

install(FILES
	${WEBEXTRACTOR_HEADERS}
	DESTINATION ${INCLUDE_INSTALL_DIR}/ddms
	)
