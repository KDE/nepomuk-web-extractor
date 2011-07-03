include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    )

add_subdirectory(graph)
add_subdirectory(decision)
add_subdirectory(webextractor)
add_subdirectory(ui)
