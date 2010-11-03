project(webextractor)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include_directories(
  gui/libwebextractorui
  libwebextractor
)

add_subdirectory( libwebextractor )
add_subdirectory( gui )
add_subdirectory( runtime )
add_subdirectory( test )


