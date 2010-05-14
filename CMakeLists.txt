project(webextractor)

cmake_minimum_required(VERSION 2.6)


add_subdirectory( libwebextractor )

include_directories(
    ${libwebextractor_SOURCE_DIR}/include
    )

add_subdirectory( service )
add_subdirectory( tests )


