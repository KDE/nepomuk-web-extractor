project(webexshell)

include_directories(
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  #libwebexsettings path
  ${libwebexsettings_BINARY_DIR}
  ${libwebexsettings_SOURCE_DIR}
  #libwebexsettingsui path
  ${libwebexsettingsui_BINARY_DIR}
  ${libwebexsettingsui_SOURCE_DIR}
  ${NEPOMUK_INCLUDE_DIR}
  ${NEPOMUK_INCLUDE_DIR}/nepomuk
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${LIBWEBEXTRACTOR_INCLUDE_DIRS}
  )

set(webexshell_SRCS
  main.cpp
  mainwindow.cpp
  resourceanalyzerthread.cpp
)

kde4_add_ui_files(webexshell_SRCS
  mainwindow.ui
)

kde4_add_executable(webexshell ${webexshell_SRCS})

target_link_libraries(webexshell
  ${NEPOMUK_LIBRARIES}
  ${QT_QTGUI_LIBRARY}
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS}
  ${SOPRANO_LIBRARIES}
  ${NEPOMUK_QUERY_LIBRARIES}
  nepomukutils
  nepomuk_decision
  nepomuk_decisionui
  webextractor
  webexsettings
  webexsettingsui
)

install(TARGETS webexshell ${INSTALL_TARGETS_DEFAULT_ARGS})
