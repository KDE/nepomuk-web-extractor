find_package(Tesseract)

add_subdirectory(debug)
add_subdirectory(autotag)
add_subdirectory(test)
add_subdirectory(tvdb)
if(TESSERACT_FOUND)
   add_subdirectory(olena)
endif()
