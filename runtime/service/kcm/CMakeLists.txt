include_directories(
    ${webextractorservice_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${libwebexsettingsui_SOURCE_DIR}
    ${libwebexsettingsui_BINARY_DIR}
    )
configure_file(kcm_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/kcm_config.h )

set( webexkcm_SRCS
    #${webexsettings_SRCS}
    main.cpp
    ConfigWidget.cpp
    GeneralPage.cpp
    #CategoriesPage.cpp
    #catnamevalidator.cpp
    #pluginItemDelegate.cpp
    #pluginItemWidget.cpp
    #pluginselector.cpp
    categorieswidget.cpp
    categoryeditor.cpp
    datapppage.cpp
    queryeditor.cpp
    pluginmodel.cpp
    setupWPage.cpp
    sourceWPage.cpp
    datappwizard.cpp
    )

kde4_add_ui_files(webexkcm_SRCS 
    categorieswidget.ui
    categoryeditor.ui
    generalPage.ui
    categoriesPage.ui
    datappPage.ui
    sourceWPage.ui
    setupWPage.ui
    pluginSelector.ui
    pluginDelegate.ui
    )

kde4_add_plugin( kcm_webexconfig
    ${webexkcm_SRCS} 
    #${webexsettings_SRCS} 
    )

target_link_libraries(kcm_webexconfig
     ${KDE4_KDECORE_LIBS}
     ${KDE4_KDEUI_LIBRARY}
     ${KDE4_KNOTIFYCONFIG_LIBRARY}
     ${KDE4_KIO_LIBS}
     ${NEPOMUK_LIBRARIES}
     ${NEPOMUK_QUERY_LIBRARIES}
     webexsettings
     webexsettingsui
     nepomukutils
     )

install(
  TARGETS kcm_webexconfig
  DESTINATION ${PLUGIN_INSTALL_DIR}
  )
########### install files ###############

install( FILES webextractorconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )
