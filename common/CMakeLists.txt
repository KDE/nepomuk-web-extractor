set( ddms_common_SRCS
    protocol_types.cpp
    )

add_definitions(-DMAKE_DDMS_COMMON_LIB)
add_library( ddms_common SHARED
    ${ddms_common_SRCS}
    )

target_link_libraries( ddms_common
    ${QT_QTDBUS_LIBRARY}
    )

install(TARGETS ddms_common
    	LIBRARY DESTINATION lib
    	RUNTIME DESTINATION bin
    	ARCHIVE DESTINATION lib
)
remove_definitions(-DMAKE_DDMS_COMMON_LIB)
