
include_directories(${CMAKE_SOURCE_DIR}/src)

add_library (raytracer
  Camera.h
  Perspective.cpp Perspective.h
  Light.h
  PointLight.cpp PointLight.h
  Model.h
  Scene.cpp Scene.h
  Sphere.cpp Sphere.h
  Triangle.cpp Triangle.h
  Box.cpp Box.h
  Shader.h
  Lambertian.cpp Lambertian.h
  BlinnPhong.cpp BlinnPhong.h
  Mirror.cpp Mirror.h
  SceneFile_Instancer.cpp SceneFile_Instancer.h
  SceneDataTypes.h)

