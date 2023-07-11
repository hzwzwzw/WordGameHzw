# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\WordGameGUI_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\WordGameGUI_autogen.dir\\ParseCache.txt"
  "WordGameGUI_autogen"
  )
endif()
