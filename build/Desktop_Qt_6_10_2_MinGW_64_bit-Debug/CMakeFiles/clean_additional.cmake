# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\spacecraftsimulator_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\spacecraftsimulator_autogen.dir\\ParseCache.txt"
  "spacecraftsimulator_autogen"
  )
endif()
