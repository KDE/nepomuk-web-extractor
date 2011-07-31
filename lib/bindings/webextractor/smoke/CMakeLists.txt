include_directories( ${LIBWEBEXTRACTOR_INCLUDE_DIRS} ${nepomuk_decision_SOURCE_DIR}  )

configure_file(config.xml.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.xml @ONLY)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/smokedata.cpp 
    ${CMAKE_CURRENT_BINARY_DIR}/x_1.cpp 
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_2.cpp 
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_3.cpp 
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_4.cpp 
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_5.cpp 
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_6.cpp 
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_7.cpp 
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_8.cpp
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_9.cpp
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_10.cpp

    COMMAND ${SMOKE_GEN_BIN}
    ARGS -config "${CMAKE_CURRENT_BINARY_DIR}/config.xml" -smokeconfig "${CMAKE_CURRENT_SOURCE_DIR}/smokeconfig.xml"
         -- ${CMAKE_CURRENT_SOURCE_DIR}/webextractor_includes.h
    DEPENDS ${SMOKE_GEN_BIN} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

macro_add_file_dependencies( ${CMAKE_CURRENT_BINARY_DIR}/smokedata.cpp ${CMAKE_CURRENT_BINARY_DIR}/x_1.cpp )

set(smokewebextractor_LIB_SRCS ${CMAKE_CURRENT_BINARY_DIR}/smokedata.cpp  
    ${CMAKE_CURRENT_BINARY_DIR}/x_1.cpp 
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_2.cpp
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_3.cpp
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_4.cpp
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_5.cpp
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_6.cpp
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_7.cpp
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_8.cpp
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_9.cpp
    #    ${CMAKE_CURRENT_BINARY_DIR}/x_10.cpp
)

kde4_add_library(smokewebextractor SHARED ${smokewebextractor_LIB_SRCS})

target_link_libraries(smokewebextractor
    ${LIBWEBEXTRACTOR_LIBRARIES}
    ${NEPOMUK_LIBRARIES}
    ${SMOKE_QTGUI_LIBRARY}
    ${SMOKE_QTCORE_LIBRARY}
    ${SMOKE_BASE_LIBRARY}
)
