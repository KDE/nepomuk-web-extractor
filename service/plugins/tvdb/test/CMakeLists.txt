project(tvdb_webex_plugin_test)

include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
  ${CMAKE_CURRENT_BINARY_DIR}
)

kde4_add_unit_test(tvshowfilenameanalyzertest NOGUI tvshowfilenameanalyzertest.cpp ../tvshowfilenameanalyzer.cpp)
target_link_libraries(tvshowfilenameanalyzertest
  ${QT_QTTEST_LIBRARY}
  ${KDE4_KDECORE_LIBS}
)
