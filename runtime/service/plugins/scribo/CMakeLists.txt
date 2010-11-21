project(webexplugin_scribo)

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
  ${SCRIBO_INCLUDE_DIR}
  )


link_directories(${LIBWEBEXTRACTOR_LIBRARY_DIRS})

set (scribo_plugin_SRCS
    scriboplugin.cpp
    scribodatappreply.cpp
    scribodatapp.cpp
    )

kde4_add_plugin(webexplugin_scribo ${scribo_plugin_SRCS})

target_link_libraries( webexplugin_scribo
    ${LIBWEBEXTRACTOR_LIBRARIES}
    webexplugin
    scribo
    ${NEPOMUK_LIBRARIES}
    ${NEPOMUK_QUERY_LIBRARIES}
    ${KDE4_NEPOMUK_LIBS}
    ${QT_LIBRARIES}
    )

install(
  TARGETS webexplugin_scribo
  DESTINATION ${PLUGIN_INSTALL_DIR}
  )

install(
  FILES webexplugin_scribo.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
  )
