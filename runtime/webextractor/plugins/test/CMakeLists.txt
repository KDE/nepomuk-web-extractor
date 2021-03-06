
project(webexplugin_test)
cmake_minimum_required(VERSION 2.6)

set (QT_MIN_VERSION "4.6.0")
set (QT_USE_QTNETWORK true)


find_package(KDE4  REQUIRED)
find_package(Nepomuk REQUIRED)

# check for required packages
if(WEBEXTRACTOR_VERSION_STRING)
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

set (test_plugin_SRCS
    testplugin.cpp
    testexecutivereply.cpp
    testexecutive.cpp
    )

set (TEST_PLUGIN_VERSION 0.1)
set (TEST_PLUGIN_INTERNAL_VERSION 0)

configure_file(testplugin_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/testplugin_config.h  )

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    )

kde4_add_plugin(webexplugin_test ${test_plugin_SRCS})

target_link_libraries( webexplugin_test
    ${LIBWEBEXTRACTOR_LIBRARIES}
    webexplugin
    ${NEPOMUK_LIBRARIES}
    ${KDE4_NEPOMUK_LIBS}
    ${QT_LIBRARIES}
    )

install(
  TARGETS webexplugin_test
  DESTINATION ${PLUGIN_INSTALL_DIR}
  )

install(
  FILES webexplugin_test.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}/webextractor
  )


