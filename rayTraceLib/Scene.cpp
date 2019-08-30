  #include "Scene.h"

void Scene::addModel(Model* newModel)
  {
  	models.push_back(newModel);
  }

bool Scene::removeModel(const Model* toDelete)
  {
  	return false;
  }

  void Scene::addLight(Light* newModel)
  {
  	return;
  }

bool Scene::removeLight(const Light* toDelete)
  {
  	return false;
  }

void Scene::setCameraLoc(const Vector3D newPos)
  {
  	camera->setLocation(newPos);
  }

void Scene::setCameraDirection(const Vector3D newDir)
  {
  	camera->setDirection(newDir);
  }

Vector3D Scene::getCameraLoc() const
  {
  	return camera->getPosition();
  }

Vector3D Scene::getCameraDir() const
  {
  	return Vector3D();
  }

void Scene::setCameraImageSize(float width, float height)
  { 
  	camera->setImageSize(width, height);
  }

void Scene::setCameraImagePixels(int width, int height)
{
  camera->setImagePixels(width, height);
}



  void Scene::drawScene(png::image < png::basic_rgb_pixel <unsigned char> > & screen)
  {
  	camera->drawScene(screen, models, lights);
  }


  void Scene::setCamera(cameraData camDat)
  {
  	if (camDat.type == perspective)
  	{
  		int pixX = camera->getImagePlanePixX();
  		int pixY = camera->getImagePlanePixY();
  		camera = new Perspective();
  		camera->setLocation(camDat.position);
  		camera->setDirection(camDat.gazeDirection);
  		camera->setFocalLength(camDat.focalLength);
  		camera->setImageSize(camDat.imagePlaneWidth, camDat.imagePlaneWidth);
  		camera->setImagePixels(pixX, pixY);
  	}
  }

  void Scene::addModel(shapeData shapeDat)
  {
  	Model * newMod;
    int shadeNum = 0;
	  for (int i = 0; i < shaders.size(); i++)
    {
      if (shapeDat.shader.name == shaders[i]->getName())
        shadeNum = i;
    }
	
  	if (shapeDat.type == sphere)
  	{
  		newMod = new Sphere(shapeDat.center, shapeDat.radius, shaders[shadeNum]);
  		models.push_back(newMod);
  	}
  	else if (shapeDat.type == triangle)
  	{
  		newMod = new Triangle( shapeDat.v0, shapeDat.v1, shapeDat.v2, shaders[shadeNum]);
  		models.push_back(newMod);
  	}
    else if (shapeDat.type == box)
    {
      newMod = new Box( shapeDat.minPt, shapeDat.maxPt, shaders[shadeNum]);
      models.push_back(newMod); 
    }

  }

  void Scene::addLight(lightData lightDat)
  {
  	Light * newLight;
  	if (lightDat.type == point)
  	{
  		newLight = new PointLight(lightDat.position, lightDat.intensity);
  		lights.push_back(newLight);
  	}
  }

  void Scene::addShader(shaderData* shadeDat)
  {
    Shader * shade = NULL;
    if (shadeDat->type == lambertian)
    {
      shade = new Lambertian(shadeDat->name, shadeDat->kd_diffuse, false);
      shaders.push_back(shade);
    }
    else if (shadeDat->type == blinnphong)
    {
      shade = new BlinnPhong(shadeDat->name, shadeDat->kd_diffuse, shadeDat->ks_specular, shadeDat->phongExp, false);
      shaders.push_back(shade);
    }
    else if (shadeDat->type == mirror)
    {
      shade = new Mirror(shadeDat->name, true);
      shaders.push_back(shade);
    }
  }
