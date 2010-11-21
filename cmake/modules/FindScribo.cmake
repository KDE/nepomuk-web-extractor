# Once done this will define
#
#  SCRIBO_FOUND - system has Scribo
#  SCRIBO_INCLUDE_DIR - the Scribo include directory
#  SCRIBO_LIBRARIES - Link these to use Scribo Query
#  SCRIBO_CLIENT_LIBRARIES - Link these to use Scribo Query Client
#  SCRIBO_DEFINITIONS - Compiler switches required for using Scribo
#

set (SCRIBO_FIND_REQUIRED ${ScriboQuery_FIND_REQUIRED})
if (SCRIBO_INCLUDE_DIR AND SCRIBO_LIBRARIES)

  # Already in cache, be silent
  set(SCRIBO_FIND_QUIETLY TRUE)

else (SCRIBO_INCLUDE_DIR AND SCRIBO_LIBRARIES)
  find_path(SCRIBO_INCLUDE_DIR
    NAMES
    scribo/textmatch.h
    PATHS
    ${KDE4_INCLUDE_DIR}
    ${INCLUDE_INSTALL_DIR}
    )

  find_library(SCRIBO_LIBRARIES
    NAMES
    scribo
    PATHS
    ${KDE4_LIB_DIR}
    ${LIB_INSTALL_DIR}
    )

  mark_as_advanced(SCRIBO_INCLUDE_DIR SCRIBO_LIBRARIES)

endif (SCRIBO_INCLUDE_DIR AND SCRIBO_LIBRARIES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SCRIBO DEFAULT_MSG 
  SCRIBO_LIBRARIES SCRIBO_INCLUDE_DIR)
