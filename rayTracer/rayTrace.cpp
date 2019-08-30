#ifndef RAYTRACERDRIVER
#define RAYTRACERDRIVER
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
#include "SceneDataTypes.h"

// Need to include the header of the code that reads and parses the
// OBJ file
#include "model_obj.h"


using namespace sivelab;




void renderSceneRT(std::string filename, GraphicsArgs args);

void renderOBJRT(std::string filename, GraphicsArgs args);



int rayTrace(int argc, char *argv[])
{
	GraphicsArgs args;
	args.process(argc, argv);

	std::string filename = args.inputFileName;

	if (filename.find(".obj") != -1 ||
		filename.find(".OBJ") != -1)
		renderOBJRT(filename, args);

	else if (filename.find(".xml") != -1 ||
				filename.find(".XML") != -1)
		renderSceneRT(filename, args);

	else
	{
		std::cerr << "Bad input file type!" << std::endl;
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}



void renderSceneRT(std::string filename, GraphicsArgs args)
{
	int w = args.width, h = args.height;
	int rpp = args.rpp;
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
  xmlScene.registerCallback("instance", sceneFactory);

  if (args.inputFileName != "")
	 xmlScene.parseFile( filename );
  else
	 {
		std::cerr << "Need input file!" << std::endl;
		exit(EXIT_FAILURE);
	 }

  //
  // Create image of scene
  //

	scene->setCameraRPP(rpp);
	png::image< png::rgb_pixel > imData( w, h );

	scene->rayTraceScene(imData);

  imData.write( filename + "Out.png" );
}



void renderOBJRT(std::string filename, GraphicsArgs args)
{
	// Store all triangles in the OBJ file in a triangle list
	// this is the primary list of all geometry in the 3D model
	// that we will build up while iterating over the structures
	// in the OBJ file data.
	std::vector<std::vector< Vector3D > > triangleList;

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
	Scene* scene = new Scene();	

	int w = args.width, h = args.height;

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
			std::vector<Vector3D> tPtr;

			tPtr.push_back(tv0);
			tPtr.push_back(tv1);
			tPtr.push_back(tv2);

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


	cameraData camDat;
	camDat.type = perspective;
	camDat.position = Vector3D(0,5,-5);
	camDat.gazeDirection = Vector3D(0,-1,1);
	camDat.focalLength = 1.0f;
	camDat.imagePlaneWidth = 0.5f;
	camDat.aspectRatio = 1.0f;
	scene->setCamera(camDat);


	lightData lD;
	lD.type = point;
	lD.position = Vector3D(15,15,15);
	lD.intensity = Vector3D(1,1,1);
	scene->addLight(lD);


	shaderData* sD = new shaderData;
	(*sD).type = lambertian;
	(*sD).name = "lamb";
	(*sD).kd_diffuse = Vector3D(1,0,0);
	scene->addShader(sD);

	scene->addFigure(triangleList, "lamb");

	for (int k = 0; k < triangleList.size(); k++)
	{
		shapeData shape;
		shape.type = triangle;
		shape.v0 = triangleList[k][0];
		shape.v1 = triangleList[k][1];
		shape.v2 = triangleList[k][2];
		shape.shader.name = "lamb";
		scene->addModel(shape);
	}

	png::image< png::rgb_pixel > imData( w, h );

	scene->rayTraceScene(imData);

	imData.write( filename + "Out.png" );
}
#endif