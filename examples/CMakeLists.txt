# 
# Adds the src directory to the include search
# path. ${CMAKE_SOURCE_DIR} is prefixed to allow the files in the
# utest directory to correctly include the relative reference to the
# src files.
#
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(test_Timer
  test_Timer.cpp
)
target_link_libraries(test_Timer sive-util)

add_executable(test_pngWrite
  test_pngWrite.cpp
)
target_link_libraries(test_pngWrite sive-util)
target_link_libraries(test_pngWrite ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_link_libraries(test_pngWrite ${PNG_LIBRARY})
target_link_libraries(test_pngWrite ${ZLIB_LIBRARY})

add_executable(test_readPNGFile
  test_readPNGFile.cpp
)
target_link_libraries(test_readPNGFile sive-util)
target_link_libraries(test_readPNGFile ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_link_libraries(test_readPNGFile ${PNG_LIBRARY})
target_link_libraries(test_readPNGFile ${ZLIB_LIBRARY})

