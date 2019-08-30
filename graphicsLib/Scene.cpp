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

void Scene::setCameraRPP(int rpp)
{
  camera->setRPP(rpp);
}



  void Scene::rayTraceScene(png::image < png::basic_rgb_pixel <unsigned char> > & screen)
  {
  	camera->rayTraceScene(screen, models, lights);
  }

  void Scene::rasterizeScene(png::image < png::basic_rgb_pixel <unsigned char> > & screen)
  {
    camera->rasterizeScene(screen, models, lights);
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
  		newMod = new Sphere(shapeDat.name, shapeDat.center, shapeDat.radius, shaders[shadeNum]);
  		models.push_back(newMod);
  	}
  	else if (shapeDat.type == triangle)
  	{
  		newMod = new Triangle(shapeDat.name, shapeDat.v0, shapeDat.v1, shapeDat.v2, shaders[shadeNum]);
  		models.push_back(newMod);
  	}
    else if (shapeDat.type == box)
    {
      newMod = new Box(shapeDat.name, shapeDat.minPt, shapeDat.maxPt, shaders[shadeNum]);
      models.push_back(newMod); 
    }
    else if (shapeDat.type == mesh)
    {
      newMod = createFigure(shapeDat.name, shapeDat.file, shaders[shadeNum]);
      models.push_back(newMod); 
    }
    else if (shapeDat.type == Sinstance)
    {
      int iNum = 0;
      for (int i = 0; i < instances.size(); i++)
        if (shapeDat.id == instances[i]->getID())
          iNum = i;
      Model* thisMod = instances[iNum];

      if (shapeDat.shader.name == "")
        newMod = new Instance(shapeDat.name, shapeDat.m, thisMod);
      else
        newMod = new Instance(shapeDat.name, shapeDat.m, thisMod, shaders[shadeNum]);
      models.push_back(newMod);
    }

  }

  void Scene::addFigure(std::vector<std::vector< Vector3D > > newList, std::string shader)
  {
    Model * newMod;
    int shadeNum = 0;
    for (int i = 0; i < shaders.size(); i++)
    {
      if (shader == shaders[i]->getName())
        shadeNum = i;
    }

    std::vector<Triangle*> tris;
    
    for (int k = 0; k < newList.size(); k++)
    {

      Triangle* newTri = new Triangle("fig", newList[k][0], newList[k][1], newList[k][2], shaders[shadeNum]);
      tris.push_back(newTri);
    }

    newMod = new Figure("fig", tris, shaders[shadeNum]);
    models.push_back(newMod);

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
    else if (shadeDat->type == glaze)
    {
      shade = new Glaze(shadeDat->name, shadeDat->kd_diffuse, true, shadeDat->mirrorCoef);
      shaders.push_back(shade);
    }
    else if (shadeDat->type == blinnPhongMirrored)
    {
      shade = new BlinnPhongMirrored(shadeDat->name, shadeDat->kd_diffuse, shadeDat->ks_specular, shadeDat->phongExp, true, shadeDat->mirrorCoef);
      shaders.push_back(shade);
    }
    else if (shadeDat->type == dielectric)
    {
      shade = new Dielectric(shadeDat->name, shadeDat->refractiveIndex, shadeDat->attenuationCoef);
      shaders.push_back(shade);
    }
  }
  

  void Scene::addInstance(shapeData insDat)
  {
    Model * inst= NULL;

    int shadeNum = 0;
    for (int i = 0; i < shaders.size(); i++)
    {
      if (insDat.shader.name == shaders[i]->getName())
        shadeNum = i;
    }

    if (insDat.type == box)
    {
      inst = new Box(insDat.name, insDat.minPt, insDat.maxPt, shaders[shadeNum]);
      instances.push_back(inst);
    }
    else if (insDat.type == sphere)
    {
      inst = new Sphere(insDat.name, insDat.center, insDat.radius, shaders[shadeNum]);
      instances.push_back(inst);
    }
    else if (insDat.type == mesh)
    {
      inst = createFigure(insDat.name, insDat.file, shaders[shadeNum]);
      instances.push_back(inst); 
    }
  }




/*
This method takes in a path to a file and a shader for the file
*/
Figure* Scene::createFigure(std::string name, std::string filename, Shader* shader)
{
  // Store all triangles in the OBJ file in a triangle list
  // this is the primary list of all geometry in the 3D model
  // that we will build up while iterating over the structures
  // in the OBJ file data.
  std::vector<Triangle*> triangleList;

  // ///////////////////////////////
  // the following code is needed for reading the OBJ files and
  // parsing out their data.

  // After reading in the entire OBJ file, we will keep a list of 
  // ALL vertices in the 3D object, these will be stored in the 
  // vertexList object. I call it a list for simplicity but
  // it is implemented as a STL vector.
  std::vector<Vector3D> vertexList;
   
  // For efficiency in large 3D models, many vertices can and are
  // re-used, especially where multiple triangles meet at a point. This
  // is managed by what is called an Indexed Triangle list. The incides
  // in this list refer to vertices in the vertexList. Sets of 3 indices
  // represent a single triangle.
  std::vector<int> indexList;

  filename = "../XMLS/" + filename;

  std::cerr << "Reading OBJ file: " << filename << "..." << std::endl;
   

  // The ModelOBJ class comes from the header model_obj.h. We will
  // use this class to read the OBJ file. After it is read, we will
  // extract the elements needed to create a list of triangles.
  ModelOBJ mOBJ;
  if (mOBJ.import( filename.c_str() ))
   std::cout << "...loading successful." << std::endl;
  else {
   std::cout << "...unsuccessful! Exiting!" << std::endl;
   exit(EXIT_FAILURE);
  }

  std::cout << "Number of meshes contained within OBJ: " << mOBJ.getNumberOfMeshes() << std::endl;
  std::cout << "Number of triangles contained within OBJ: " << mOBJ.getNumberOfTriangles() << std::endl;

  const ModelOBJ::Mesh *pMesh = 0;
  const ModelOBJ::Vertex *pVertices = 0;

  const int *idxBuffer = mOBJ.getIndexBuffer();

  // OBJ files can contain multiple "meshes". Each mesh can contain a
  // variety of triangles. Here we will loop over all the meshes and
  // extract out all the triangles in each mesh.
  for (int mIdx=0; mIdx<mOBJ.getNumberOfMeshes(); mIdx++) {

     // Obtain pointers to the mesh data and the vertex list
     pMesh = &mOBJ.getMesh(mIdx);
     pVertices = mOBJ.getVertexBuffer();

     // Some meshes come with shading data embedded in them, such as
     // diffuse and specular components. I am leaving this out for now,
     // but if you find yourself interested, you could extract the
     // material properties out of the mesh like this:
     // 
     // const ModelOBJ::Material *pMaterial = 0;
     // pMaterial = pMesh->pMaterial;
     // std::cout << "Diffuse component: " << pMaterial->diffuse[0]
     // << ", " << pMaterial->diffuse[1]
     // << ", " << pMaterial->diffuse[2] << std::endl;

     // Iterate over all indices in this mesh
     for (int i=pMesh->startIndex; i<(pMesh->startIndex + pMesh->triangleCount*3); i+=3) {

        // The model_obj.h class contains it's OWN Vertex class. We
        // extract the three vertices using the indices here to make it
        // obvious where the data is coming from
        ModelOBJ::Vertex v0, v1, v2;
        v0 = pVertices[ idxBuffer[i] ];
        v1 = pVertices[ idxBuffer[i+1] ];
        v2 = pVertices[ idxBuffer[i+2] ];
   
        // Place these three vertices into our Vector3D class which is
        // used for the Triangle class.
        Vector3D tv0(v0.position[0],v0.position[1],v0.position[2]);
        Vector3D tv1(v1.position[0],v1.position[1],v1.position[2]);
        Vector3D tv2(v2.position[0],v2.position[1],v2.position[2]);

        // Note that with the following step, we are losing the
        // efficiency of the indexed triangle list since we end up making
        // individual triangles without sharing data. If you are
        // interested in changing this in your own code, go for it! Let me
        // know if you do this!
        // 
        // Using the actual vertices of the triangle, instance a new triangle!
        Triangle *tPtr = new Triangle(name, tv0, tv1, tv2, shader );

        // In case you're interested, the OBJ files also provide the
        // normal vectors at each vertex. You can access them like this:
        // 
        // v0.normal[0]
        // v0.normal[1]
        // v0.normal[2]
        // and so on for each vertex

        // be sure to add this triangle onto the triangle list
        triangleList.push_back(tPtr);
      } 
  }

  Figure * fig = new Figure(name, triangleList, shader);
  return fig;
}
