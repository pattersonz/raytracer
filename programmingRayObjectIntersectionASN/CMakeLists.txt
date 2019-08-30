project(RayObjectIntersection)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/rayTraceLib)

add_executable(rayObjectIntersection
  driver.cpp)

target_link_libraries(rayObjectIntersection raytracer)
target_link_libraries(rayObjectIntersection sive-util)
target_link_libraries(rayObjectIntersection ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_link_libraries(rayObjectIntersection ${PNG_LIBRARY})
target_link_libraries(rayObjectIntersection ${ZLIB_LIBRARY})
