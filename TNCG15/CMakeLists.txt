# CMakeList.txt : CMake project for TNCG15, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (TNCG15 "main.cpp" "TNCG15.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TNCG15 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
