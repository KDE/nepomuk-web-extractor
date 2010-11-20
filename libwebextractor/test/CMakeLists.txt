enable_testing()


## ==============  DUMMY SERVICE DATA BACKEND TEST =================== 
set( DUMMYSDBTEST_SRCS
    dummyservicedatabackendtest.cpp
    )

QT4_WRAP_CPP( DUMMYSDBTEST_MOC ${DUMMYSDBTEST_SRCS} )

kde4_add_executable(dummyservicedatabackendtest ${DUMMYSDBTEST_SRCS} )
target_link_libraries( dummyservicedatabackendtest
    webextractor
  ${NEPOMUK_LIBRARIES}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTTEST_LIBRARY}
  )

add_test( dummysdbtest dummyservicedatabackendtest )
## ==================================================================


##  ======================= TIMEOUT TEST ============================
set( DATAPPTIMEOUT_SRCS
    datapptimeouttest.cpp
    )
#QT4_WRAP_CPP( DATAPPTIMEOUT_MOC ${DATAPPTIMEOUT_SRCS} )

kde4_add_executable(timeoutreplytest ${DATAPPTIMEOUT_SRCS} )


target_link_libraries( timeoutreplytest
    webextractor
  ${NEPOMUK_LIBRARIES}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTTEST_LIBRARY}
  )

add_test( datapptimeout timeoutreplytest )

## ================== DECISION TEST =================================
set( DECISION_SRCS
    decisiontest.cpp
    )
#QT4_WRAP_CPP( DECISION_MOC ${DECISION_SRCS} )
kde4_add_executable(decisiontest ${DECISION_SRCS} )

target_link_libraries( decisiontest
    webextractor
  ${NEPOMUK_LIBRARIES}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTTEST_LIBRARY}
  )

add_test( decisiontest decisiontest )
