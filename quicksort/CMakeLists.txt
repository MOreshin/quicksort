# CMakeList.txt : CMake project for quicksort, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (quicksort "quicksort.cpp" "quicksort.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET quicksort PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
