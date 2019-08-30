
include_directories(${CMAKE_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
link_libraries (${OPENGL_LIBRARY})

add_library (graphicsLib
  Matrix4x4.cpp Matrix4x4.h
  Camera.h
  Perspective.cpp Perspective.h
  Light.h
  PointLight.cpp PointLight.h
  BVA.cpp BVA.h
  Model.cpp Model.h
  Instance.cpp Instance.h
  Scene.cpp Scene.h
  Sphere.cpp Sphere.h
  Triangle.cpp Triangle.h
  Box.cpp Box.h
  Figure.cpp Figure.h
  Shader.h
  Lambertian.cpp Lambertian.h
  BlinnPhong.cpp BlinnPhong.h
  BlinnPhongMirrored.cpp BlinnPhongMirrored.h
  Mirror.cpp Mirror.h
  Glaze.cpp Glaze.h
  Dielectric.cpp Dielectric.h
  SceneFile_Instancer.cpp SceneFile_Instancer.h
  SceneDataTypes.h
  GLSL.cpp GLSL.h)

