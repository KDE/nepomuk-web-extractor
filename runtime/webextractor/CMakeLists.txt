add_subdirectory(libwebexsettings)
add_subdirectory(libwebexsettingsui)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/libwebexsettings
    ${CMAKE_CURRENT_BINARY_DIR}/libwebexsettingsui
    ${CMAKE_CURRENT_SOURCE_DIR}/libwebexsettings
    ${CMAKE_CURRENT_SOURCE_DIR}/libwebexsettingsui
    )

add_subdirectory(service)
add_subdirectory(plugins)
add_subdirectory(console)
add_subdirectory(kcm)
add_subdirectory(webexshell)

