kde4_add_executable( decision_test
    decision_test.cpp
    )

target_link_libraries(decision_test
    ${QT_QTCORE_LIBRARY}
    ${QT_QTTEST_LIBRARY}
    nepomukdatamanagement
    nepomuk_decision
    )

add_test(test_decision decision_test)

