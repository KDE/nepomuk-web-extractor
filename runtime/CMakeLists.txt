# Attention! service and console depends from
# libwebexsettingsui and libwebexsettings, but these are
# static non-instalable libraries. 
# They must be built before service and console
# or make a shared one
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib
    ${CMAKE_CURRENT_BINARY_DIR}/../lib
    ${nepomuk_decision_SOURCE_DIR}
    ${nepomuk_decision_BINARY_DIR}
    ${nepomuk_decisionui_SOURCE_DIR}
    ${nepomuk_decisionui_BINARY_DIR}
    ${nepomukgraph_SOURCE_DIR}
    ${nepomukgraph_BINARY_DIR}
    ${nepomuk_webextractor_SOURCE_DIR}
    ${nepomuk_webextractor_BINARY_DIR}
    )

add_subdirectory(webextractor)
add_subdirectory(graphviz)
