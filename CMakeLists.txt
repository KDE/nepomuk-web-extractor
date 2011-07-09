project(webextractor)

cmake_minimum_required(VERSION 2.6)

set (QT_MIN_VERSION "4.6.0")
set (QT_USE_QTNETWORK true)
find_package(KDE4  REQUIRED)
find_package(Qt4  REQUIRED)
find_package(Nepomuk REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

## Prepare variables for building metapackage
set( BUILD_METAPACKAGE 1)


include_directories(
  ${NEPOMUK_INCLUDE_DIR}
  ${KDE4_INCLUDES}
  )

enable_testing()

# First build libraries
add_subdirectory( lib)
# Then extracto all necessary config variables from libraries

set( LIBWEBEXTRACTOR_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/webextractor
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/webextractor/resourceservicedata
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/graph
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/decision
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/
    ${CMAKE_CURRENT_BINARY_DIR}/lib/
    ${CMAKE_CURRENT_BINARY_DIR}
    )
set( LIBWEBEXTRACTORUI_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ui/
    ${CMAKE_CURRENT_BINARY_DIR}/lib/ui/
    )

add_subdirectory( runtime )
add_subdirectory( test )
