set (debug_plugin_SRCS
    debug_plugin.cpp
    simple_debug_reply.cpp
    )

set (DEBUG_PLUGIN_VERSION "0.1")
set (DEBUG_PLUGIN_INTERNAL_VERSION 0)

configure_file(debug_plugin_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/debug_plugin_config.h)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    )

kde4_add_plugin(webexplugin_debug ${debug_plugin_SRCS})

target_link_libraries( webexplugin_debug
    ${LIBWEBEXTRACTOR_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    webexplugin
    )

install(
  TARGETS webexplugin_debug
  DESTINATION ${PLUGIN_INSTALL_DIR}
  )

install(
  FILES webexplugin_debug.desktop
  DESTINATION ${SERVICES_INSTALL_DIR}/webextractor
  )

