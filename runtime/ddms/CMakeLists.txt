project(ddmsservice)


add_subdirectory(common)
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    )

add_subdirectory(service)
add_subdirectory(client)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/client
    ${CMAKE_CURRENT_SOURCE_DIR}/client
    )
add_subdirectory(gui)
