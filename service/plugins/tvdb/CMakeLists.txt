
project(webexplugin_tvdb)

set (QT_MIN_VERSION "4.6.0")
set (QT_USE_QTNETWORK true)
find_package(KDE4  REQUIRED)
find_package(Nepomuk REQUIRED)

find_package(LibTvdb 0.2 QUIET)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibTvdb DEFAULT_MSG LIBTVDB_INCLUDE_DIR)
macro_log_feature(LibTvdb_FOUND "LibTvdb" "Small lib to fetch data from thetvdb.com" "http://libtvdb.sourceforge.net" TRUE 0.2 "Necessary to access TV show information from thetvdb.com.")

# check for required packages
if(WEBEXTRACTOR_VERSION_STRING)
    # if WEBEXTRACTOR_VERSION_STRING is defined, we are compiling the meta-package
    set(LIBWEBEXTRACTOR_INCLUDE_DIRS
        ../libwebextractor/include
        ${CMAKE_CURRENT_BINARY_DIR}/../libwebextractor/include)
    set(LIBWEBEXTRACTOR_LIBRARIES webextractor)
else(WEBEXTRACTOR_VERSION_STRING)
    find_package(LibWebextractor REQUIRED)
endif(WEBEXTRACTOR_VERSION_STRING)


include_directories(
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${NEPOMUK_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${LIBWEBEXTRACTOR_INCLUDE_DIRS}
  )


link_directories(${LIBWEBEXTRACTOR_LIBRARY_DIRS})

set (tvdb_plugin_SRCS
    tvdbplugin.cpp
    tvdbdatappreply.cpp
    tvdbdatapp.cpp
    tvshowfilenameanalyzer.cpp
    )

set (TVDB_PLUGIN_VERSION 0.1)

configure_file(tvdbplugin_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/tvdbplugin_config.h  )

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    )

kde4_add_plugin(webexplugin_tvdb ${tvdb_plugin_SRCS})

target_link_libraries( webexplugin_tvdb
    ${LIBWEBEXTRACTOR_LIBRARIES}
    webexplugin
    ${NEPOMUK_LIBRARIES}
    ${KDE4_NEPOMUK_LIBS}
    ${QT_LIBRARIES}
    )

install(
  TARGETS webexplugin_tvdb
  DESTINATION ${PLUGIN_INSTALL_DIR}
  )

install(
  FILES webexplugin_tvdb.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
  )

add_subdirectory(test)

macro_display_feature_log()
