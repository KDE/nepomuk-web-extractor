# cmake macro to test if we use tesseract
#
#  TESSERACT_FOUND - system has TESSERACT libs
#  TESSERACT_INCLUDE_DIR - the TESSERACT include directory
#  TESSERACT_LIBRARY - The libraries needed to use TESSERACT

# Copyright (c) 2009-2010, Sebastian Trueg <trueg@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include(CheckCXXSymbolExists)

if (TESSERACT_INCLUDE_DIR AND TESSERACT_LIBRARY)
  # Already in cache, be silent
  set(Tesseract_FIND_QUIETLY TRUE)
endif (TESSERACT_INCLUDE_DIR AND TESSERACT_LIBRARY)

FIND_PATH(TESSERACT_INCLUDE_DIR tesseract/baseapi.h)

FIND_LIBRARY(TESSERACT_LIBRARY NAMES tesseract_api)

if(TESSERACT_LIBRARY)
  find_package(TIFF)
  if(TIFF_FOUND)
    set(CMAKE_REQUIRED_LIBRARIES ${TESSERACT_LIBRARY} ${TIFF_LIBRARY} pthread stdc++)
  else(TIFF_FOUND)
    set(CMAKE_REQUIRED_LIBRARIES ${TESSERACT_LIBRARY} pthread stdc++)
  endif(TIFF_FOUND)

  set(CMAKE_REQUIRED_INCLUDES ${TESSERACT_INCLUDE_DIR})
endif(TESSERACT_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Tesseract DEFAULT_MSG TESSERACT_INCLUDE_DIR TESSERACT_LIBRARY )

MARK_AS_ADVANCED(TESSERACT_INCLUDE_DIR TESSERACT_LIBRARY)
