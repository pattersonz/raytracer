#ifndef __SCENEH__
#define __SCENEH__
#include <vector>
#include "Camera.h"
#include "Perspective.h"
#include "Light.h"
#include "PointLight.h"
#include "Model.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Box.h"
#include "Shader.h"
#include "Lambertian.h"
#include "BlinnPhong.h"
#include "Mirror.h"
#include "Vector3D.h"
#include "png++/png.hpp"
#include "SceneDataTypes.h"

using namespace sivelab;

class Scene
{
 private:
  std::vector <Light *> lights; //list of all light sources in the scene
  std::vector <Model *> models; //list of all objects in the scene
  std::vector <Shader *> shaders; //list of all the shaders in the scene
  Camera * camera; //main camera
 public:

  /*
    Default Constructor
   */
  Scene()
  {
    camera = new Perspective();
  }
  
  /*
    Adds a new model to the list of models in the
    scene.
   */
  void addModel(Model* newModel);

  /*
    Finds a specified model and removes it from
    the list of models. Returns if the model has
    been found or not.
   */
  bool removeModel(const Model* toDelete);

  /*
    Adds a new light to the list of models in the
    scene.
   */
  void addLight(Light* newModel);

  /*
    Finds a specified light and removes it from
    the list of models. Returns if the model has
    been found or not.
   */
  bool removeLight(const Light* toDelete);


  /*
    Sets a new location for the camera.
   */
  void setCameraLoc(const Vector3D newPos);

  /*
    Sets a new direction for the camera to be
    facing. (-w direction)
   */
  void setCameraDirection(const Vector3D newDir);

  /*
    Returns the current location of the camera.
   */
  Vector3D getCameraLoc() const;

  /*
    Returns the current direction the camera is
    facing.
   */
  Vector3D getCameraDir() const;

  /*
    Sets a new image size
   */
  void setCameraImageSize(float width, float height);

  /*
   Specifies a new pixel count
  */
  void setCameraImagePixels(int width, int height);


  /*
    calculates the color at each pixel of the image plane
   */
  void drawScene(png::image < png::basic_rgb_pixel <unsigned char> > & screen);


  /*
	takes in a cameraData structure to set values to the camera
  */
  void setCamera(cameraData camDat);

  /*
	takes in a shapeData structure to add to the list
  */
  void addModel(shapeData shapeDat);

  /*
  	takes in a lightData structure to add to the list
  */
  void addLight(lightData lightDat);

  /*
    takes in a shaderData structure to add to the list
  */
  void addShader(shaderData* shadeDat);
};
#endif
