add_definitions(-DMAKE_DDMS_CLIENT_LIB)
set( ddms_client_SRCS
    client.cpp
    respond_proxies.cpp
    ../common/decisionmanagementserviceproxy.cpp
    decision_proxy.cpp
    decision_proxy_data.cpp
    decision_model.cpp
    )

set ( ddms_client_HEADERS
    client.h
    decision_proxy.h
    decision_model.h
    respond_proxies.h
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
remove_definitions(-DMAKE_DDMS_CLIENT_LIB)
