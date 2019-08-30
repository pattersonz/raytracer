/*
 */

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cassert>

#include "png++/png.hpp"
#include "handleGraphicsArgs.h"
#include "XMLSceneParser.h"
#include "SceneFile_Instancer.h"

#include "Vector3D.h"
#include "Scene.h"
#include "Camera.h"
#include "Model.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"


using namespace sivelab;

int main(int argc, char *argv[])
{
  GraphicsArgs args;
  args.process(argc, argv);
  int w = args.width, h = args.height;

  //parse XML
  XMLSceneParser xmlScene;
  Scene* scene = new Scene();
  scene->setCameraImagePixels(w,h);

  SceneFile_Instancer * sceneFactory = new SceneFile_Instancer(scene);

  // register object creation function with xmlScene
  xmlScene.registerCallback("camera", sceneFactory);
  xmlScene.registerCallback("light", sceneFactory);
  xmlScene.registerCallback("shader", sceneFactory);
  xmlScene.registerCallback("shape", sceneFactory);

  if (args.inputFileName != "")
    xmlScene.parseFile( args.inputFileName );
  else
    {
      std::cerr << "Need input file!" << std::endl;
      exit(EXIT_FAILURE);
    }

  //
  // Create image of scene
  //

  png::image< png::rgb_pixel > imData( w, h );
  
  scene->drawScene(imData);
  
  imData.write( args.inputFileName + "Out.png" );

  exit(EXIT_SUCCESS);
}


