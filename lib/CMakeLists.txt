include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    )

option( BUILD_API_DOCS "Build API documentation for libraries." ON )

if ( BUILD_API_DOCS )
    find_package(Doxygen REQUIRED )
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        )

endif (BUILD_API_DOCS)


add_subdirectory(graph)
add_subdirectory(decision)
add_subdirectory(webextractor)
add_subdirectory(ui)
add_subdirectory(bindings)
