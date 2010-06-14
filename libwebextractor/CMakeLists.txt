project(libwebextractor)

cmake_minimum_required(VERSION 2.6)

set (QT_MIN_VERSION "4.6.0")
set (QT_USE_QTNETWORK true)


find_package(Qt4  REQUIRED)
find_package(KDE4  REQUIRED)
find_package(Nepomuk REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

set(WEBEXTRACTOR_VERSION_MAJOR 0 CACHE INT "Major Webextractor version number" FORCE)
set(WEBEXTRACTOR_VERSION_MINOR 0 CACHE INT "Minor Webextractor version number" FORCE)
set(WEBEXTRACTOR_VERSION_PATCH 1 CACHE INT "Release Webextractor version number" FORCE)
set(WEBEXTRACTOR_VERSION_STRING "${WEBEXTRACTOR_VERSION_MAJOR}.${WEBEXTRACTOR_VERSION_MINOR}.${WEBEXTRACTOR_VERSION_PATCH}" CACHE STRING "Webextractor version string" FORCE)
set( LIBWEBEXTRACTOR_VERSION $WEBEXTRACTOR_VERSION_STRING})

# definition of LIB_DESTINATION which is the path where the lib is installed
set (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
set (LIB_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}" CACHE STRING "Library directory name" FORCE)

configure_file(LibWebextractorConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/LibWebextractorConfig.cmake @ONLY )

include_directories(
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/webextractor
  ${CMAKE_CURRENT_BINARY_DIR}
  ${NEPOMUK_INCLUDE_DIR}
  ${CMAKE_BUILD_DIR}
  ${KDE4_INCLUDES}
  )


add_subdirectory( lib )




#installing

file (GLOB WEBEXTRACTOR_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/webextractor/*.h ) 
#set ( WEBEXTRACTOR_HEADERS 
#    ${CMAKE_CURRENT_SOURCE_DIR}/include/webextractor/resourceanalyzerfactory.h 
#    ${CMAKE_CURRENT_SOURCE_DIR}/include/webextractor/resourceanalyzer.h
#    )

install(FILES
	${WEBEXTRACTOR_HEADERS}
	DESTINATION include/webextractor
	)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LibWebextractorConfig.cmake
	DESTINATION ${LIB_DESTINATION}/libwebextractor)
