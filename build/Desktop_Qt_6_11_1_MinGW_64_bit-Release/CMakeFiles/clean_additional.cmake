# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\Tuack-Qt_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Tuack-Qt_autogen.dir\\ParseCache.txt"
  "Tuack-Qt_autogen"
  )
endif()
