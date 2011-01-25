project(webextractor)

cmake_minimum_required(VERSION 2.6)

set (QT_MIN_VERSION "4.6.0")
set (QT_USE_QTNETWORK true)
find_package(KDE4  REQUIRED)
find_package(Qt4  REQUIRED)
find_package(Nepomuk REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

include_directories(
  gui/libwebextractorui
  libwebextractor
)

set( backupsynclegacy_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libbackupsynclegacy )

add_subdirectory( libbackupsynclegacy )
add_subdirectory( libwebextractor )
add_subdirectory( gui )
add_subdirectory( runtime )
add_subdirectory( test )
add_subdirectory( webexshell )
