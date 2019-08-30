include_directories(${CMAKE_SOURCE_DIR}/src)

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
link_libraries (${OPENGL_LIBRARY})

project(Render)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/graphicsLib)
include_directories(${CMAKE_SOURCE_DIR}/OpenGL)
include_directories(${CMAKE_SOURCE_DIR}/rayTracer)
include_directories(${CMAKE_SOURCE_DIR}/rasterize)

add_executable (Render
  render.cpp 
)

target_link_libraries(Render rasterize)
target_link_libraries(Render rayTracer)
target_link_libraries(Render openGL)
target_link_libraries(Render graphicsLib)
target_link_libraries(Render sive-util)
target_link_libraries(Render ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_link_libraries(Render GLEW )
target_link_libraries(Render glfw ${GLFW_STATIC_LIBRARIES})
target_link_libraries(Render ${PNG_LIBRARIES})
target_link_libraries(Render z)
target_link_libraries(Render ${ZLIB_LIBRARY})
