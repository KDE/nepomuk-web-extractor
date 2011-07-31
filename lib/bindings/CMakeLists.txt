find_package(Smoke COMPONENTS QtCore QtGui QtXml QtOpenGl QtSql QtNetwork QtDbus QtSvg)

set(QTDEFINES_FILE ${SMOKE_GEN_SHARED}/qtdefines CACHE FILEPATH "Path to the qtdefines file")
if(NOT EXISTS ${QTDEFINES_FILE})
    message(FATAL_ERROR "${QTDEFINES_FILE} doesn't exist. Please adjust the path manually")
endif(NOT EXISTS ${QTDEFINES_FILE})
add_subdirectory(webextractor)
