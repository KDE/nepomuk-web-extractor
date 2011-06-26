project(ddmsservice)

set (QT_MIN_VERSION "4.6.0")
set (QT_USE_QTNETWORK true)
find_package(KDE4  REQUIRED)
find_package(Qt4  REQUIRED)
#find_package(Nepomuk REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

include_directories(
  ${KDE4_INCLUDES}
  )

add_subdirectory(common)
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    )

add_subdirectory(service)
add_subdirectory(client)
