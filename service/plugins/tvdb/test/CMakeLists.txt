project(tvdb_webex_plugin_test)

include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
)

kde4_add_executable(filenameanalyzer filenameanalyzer.cpp ../tvshowfilenameanalyzer.cpp)
target_link_libraries(filenameanalyzer
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS}
)
