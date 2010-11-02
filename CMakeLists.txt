project(webextractor)

cmake_minimum_required(VERSION 2.6)

include_directories(
  gui/libwebextractorui
  libwebextractor
)

add_subdirectory( libwebextractor )
add_subdirectory( gui )
add_subdirectory( runtime )
add_subdirectory( test )


