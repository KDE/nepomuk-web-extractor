include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    )

set( ddms_manager_SRCS
    mainwindow.cpp
    main.cpp
    )

kde4_add_ui_files(ddms_manager_UI
    mainWindow.ui
    )

qt4_add_resources( ddms_manager_RES
    main.qrc
    )


kde4_add_executable( nepomuk_decision_manager
    ${ddms_manager_SRCS}
    ${ddms_manager_UI}
    ${ddms_manager_RES}
    )

target_link_libraries(
    nepomuk_decision_manager
    ${KDE4_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}
    ddms_client
    )

install( TARGETS nepomuk_decision_manager
    	LIBRARY DESTINATION lib
    	RUNTIME DESTINATION bin
    	ARCHIVE DESTINATION lib
	)

