# To unify building process for in-tree plugins
# and out-tree plugins, we provide some
# variables, pretending that we have
# run find_package(WebExtractor)

set( LIBWEBEXTRACTOR_LIBRARIES
    webextractor
    nepomuk_decision
    nepomukdatamanagement
    )

# Debug is disabled because
# Simple Network Executive subsystem
# is disabled
#add_subdirectory(debug)

add_subdirectory(autotag)
#add_subdirectory(test)
#add_subdirectory(tvdb)
