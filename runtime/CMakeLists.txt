# Attention! service and console depends from
# libwebexsettingsui and libwebexsettings, but these are
# static non-instalable libraries. 
# They must be built before service and console
# or make a shared one
add_subdirectory(libwebexsettings)
add_subdirectory(libwebexsettingsui)
add_subdirectory(service)
add_subdirectory(console)
add_subdirectory(graphviz)
