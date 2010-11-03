project(webexplugin_olena)

include_directories(
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${NEPOMUK_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${LIBWEBEXTRACTOR_INCLUDE_DIRS}
  ${TESSERACT_INCLUDE_DIR}
  )

add_definitions(-DNDEBUG -DHAVE_TESSERACT_3)

link_directories(${LIBWEBEXTRACTOR_LIBRARY_DIRS})

set (olena_plugin_SRCS
    olenaplugin.cpp
    olenadatappreply.cpp
    olenadatapp.cpp
    )

set (OLENA_PLUGIN_VERSION 0.1)
set (OLENA_PLUGIN_INTERNAL_VERSION 1)

configure_file(olenaplugin_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/olenaplugin_config.h  )

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    )

kde4_add_plugin(webexplugin_olena ${olena_plugin_SRCS})

target_link_libraries( webexplugin_olena
    ${LIBWEBEXTRACTOR_LIBRARIES}
    webexplugin
    ${NEPOMUK_LIBRARIES}
    ${KDE4_NEPOMUK_LIBS}
    ${QT_LIBRARIES}
    ${TESSERACT_LIBRARY}
    )


install(
  TARGETS webexplugin_olena
  DESTINATION ${PLUGIN_INSTALL_DIR}
  )

install(
  FILES webexplugin_olena.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}
  )

