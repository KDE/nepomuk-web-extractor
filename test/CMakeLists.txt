project( webextractor_tests)
set (QT_MIN_VERSION "4.6.0")


find_package(KDE4  REQUIRED)
# check for required packages
if(WEBEXTRACTOR_VERSION_STRING)
	# if WEBEXTRACTOR_VERSION_STRING is defined, we are compiling the meta-package
	set(LIBWEBEXTRACTOR_INCLUDE_DIRS
		../libwebextractor/include
		${CMAKE_CURRENT_BINARY_DIR}/../libwebextractor/include)
	set(LIBWEBEXTRACTOR_LIBRARIES webextractor)
else(WEBEXTRACTOR_VERSION_STRING)
	find_package(LibWebextractor REQUIRED)
endif(WEBEXTRACTOR_VERSION_STRING)

include_directories(
    ${LIBWEBEXTRACTOR_INCLUDE_DIRS}
    ${KDE4_INCLUDES}
    )

link_directories(${LIBWEBEXTRACTOR_LIBRARY_DIRS})

#add_executable( test_launch test_launch.cpp)
#target_link_libraries( test_launch 
#    ${LIBWEBEXTRACTOR_LIBRARIES}
#    )
