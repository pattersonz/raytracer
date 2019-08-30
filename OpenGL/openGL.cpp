#ifndef OPENGLDRIVER_
#define OPENGLDRIVER_
#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "handleGraphicsArgs.h"
#include "Model.h"
#include "Figure.h"
#include "Triangle.h"
#include "Vector3D.h"
#include "Camera.h"
#include "Perspective.h"
#include "png++/png.hpp"

#include "model_obj.h"

#include "GLSL.h"

using namespace sivelab;


void glfwErrorCallback(int error, const char* description)
{
	std::cerr << "GLFW ERROR: " << description << std::endl;
}

void addFigures(std::string filename, std::vector<Figure*>* figures, sivelab::GLSLObject s, GLuint texID, glm::mat4 modelMatrix, int scale = 1);

void renderAll(std::vector<Figure*>* figures, glm::mat4 projMatrix, glm::mat4 viewMatrix);

/**
 * main entry function - Establishes the OpenGL context with the GLFW
 * (http://www.glfw.org/) and creates a window within this context.
 *
 * 
 */
int openGL(int argc, char *argv[])
{
	GraphicsArgs args;
	args.process(argc, argv);

	std::string filename = args.inputFileName;

	if(filename.find(".obj") == -1 &&
		filename.find(".OBJ") == -1 &&
		filename != "test")
	{
		std::cout << "bad input\n";
		exit(EXIT_FAILURE);
	}


	// Initialize the GLFW library
	if ( !glfwInit() )
		exit(EXIT_FAILURE);

	// Request a specific OpenGL context when the window is created
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetErrorCallback( glfwErrorCallback );
		
	/* Create a windowed mode window and its OpenGL context */
	int winWidth = 1200;
	float aspectRatio = 8.0 / 12.0; // winWidth / (float)winHeight;
	int winHeight = winWidth * aspectRatio;
	
	GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "OpenGL Example", NULL, NULL);
	if (!window) {
		std::cerr << "ERROR: GLFW did not create a window!" << std::endl;
		
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	glewExperimental=true;
	glewInit();

	const GLubyte* renderer = glGetString (GL_RENDERER);
	const GLubyte* version = glGetString (GL_VERSION);
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported: " << version << std::endl;
	std::cout << "GLFW Version String: " << glfwGetVersionString() << std::endl;

	// Need to tell OpenGL how big the window is going to be so it can
	// create enough memory on the graphics card to store the pixel
	// colors.
	//
	// We will query glfw to see how big the window was when we
	// created it and then pass that info to OpenGL's viewport function.
	int fb_width, fb_height;
	glfwGetFramebufferSize(window, &fb_width, &fb_height);
	glViewport(0, 0, fb_width, fb_height);

	// Set the background color for our window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	sivelab::GLSLObject shaderNormalColor;
	shaderNormalColor.addShader( "../OpenGL/vertexShader_perVertexNormalColor.glsl", sivelab::GLSLObject::VERTEX_SHADER );
	shaderNormalColor.addShader( "../OpenGL/fragmentShader_vertexNormalColorBlend.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
	shaderNormalColor.createProgram();

	sivelab::GLSLObject shaderVertexColor;
	shaderVertexColor.addShader( "../OpenGL/vertexShader_perVertexColor.glsl", sivelab::GLSLObject::VERTEX_SHADER );
	shaderVertexColor.addShader( "../OpenGL/fragmentShader_vertexColorBlend.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
	shaderVertexColor.createProgram();

	sivelab::GLSLObject shaderLambertian;
	shaderLambertian.addShader( "../OpenGL/vertexShader_lambertian.glsl", sivelab::GLSLObject::VERTEX_SHADER );
	shaderLambertian.addShader( "../OpenGL/fragmentShader_lambertian.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
	shaderLambertian.createProgram();

	sivelab::GLSLObject shaderBlinnPhong;
	shaderBlinnPhong.addShader( "../OpenGL/vertexShader_blinnphong.glsl", sivelab::GLSLObject::VERTEX_SHADER );
	shaderBlinnPhong.addShader( "../OpenGL/fragmentShader_blinnphong.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
	shaderBlinnPhong.createProgram();

	sivelab::GLSLObject shaderTexture;
	shaderTexture.addShader( "../OpenGL/vertexShader_Texture.glsl", sivelab::GLSLObject::VERTEX_SHADER );
	shaderTexture.addShader( "../OpenGL/fragmentShader_Texture.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
	shaderTexture.createProgram();


//TEXTURE CODE------------------------------
	std::string texFilename[4] = {"wood.png", "gold.png", "scale.png", "fur.png"};
	GLuint texID[4] = {0,1,2,3};

	for (int i = 0; i < 4; i++)
	{
		std::cout << "Reading texture map data from file: " << texFilename[i] << std::endl;
		png::image< png::rgb_pixel > texPNGImage;
		texPNGImage.read( texFilename[i] );

		int pngWidth = texPNGImage.get_width();
		int pngHeight = texPNGImage.get_height();
		float *copiedPNGData = new float[ pngHeight * pngWidth * 3 ];

		int tIdx = 0;
		for (unsigned int idx=0; idx<pngHeight*pngWidth; ++idx) {
		  size_t col = idx % pngWidth;
		  size_t row = static_cast<size_t>( floor(idx / static_cast<float>(pngWidth)) );
		 
		  png::rgb_pixel texel = texPNGImage[ pngHeight-1 - row ][col];
		 
		  copiedPNGData[tIdx++] = texel.red/255.0;
		  copiedPNGData[tIdx++] = texel.green/255.0;
		  copiedPNGData[tIdx++] = texel.blue/255.0;
		}


		glGenTextures(1, &texID[i]);

		glBindTexture(GL_TEXTURE_2D, texID[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pngWidth, pngHeight, 0, GL_RGB, GL_FLOAT, copiedPNGData);
		glBindTexture(GL_TEXTURE_2D, 0);

		delete [] copiedPNGData;
	}	 

//---------------------------------------------



	std::vector<Figure*>* figures = new std::vector<Figure *>;


	if (filename == "test")
	{
		float id[16] = {1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1};
		glm::mat4 modelMatrix = glm::make_mat4(id);

		//T1
		Triangle* t1 = new Triangle();
		Vector3D a(-8.0f, 8.0f, 0.0f), b( 0.0f, -8.0f, 0.0f), c(8.0f, 8.0f, 0.0f);
		Vector3D aN(0.0f, 0.0f, 1.0f), bN(0.0f, 0.0f, 1.0f), cN(0.0f, 0.0f, 1.0f);
		Vector3D aC(1.0f, 0.0f, 1.0f), bC(1.0f, 0.0f, 0.0f), cC(0.0f, 0.0f, 1.0f);

		t1->glInit(a,aN, aC, glm::vec2(0,0), b,bN, bC, glm::vec2(.5,1),c,cN, cC, glm::vec2(1,0), texID[0]);
		std::vector<Triangle *> ts;
		ts.push_back(t1);
		Figure * fig = new Figure();
		fig->glInit(ts, modelMatrix, shaderTexture);
		figures->push_back(fig);
	}
	else
	{
		std::string fName[4] = {"../OBJS/table.obj", "../OBJS/feline.obj", "../OBJS/dragon.obj", "../OBJS/bunny.obj"};
		glm::mat4 modelMat[4] ={  glm::mat4(1,0,0,0,
											0,1,0,0,
											0,0,1,0,
											0,0,0,1),
								  glm::transpose(glm::mat4(-3,0,0,5,
											0,3,0,0,
											0,0,-3,0,
											0,0,0,1)),
								  glm::transpose(glm::mat4(-4,0,0,-6,
											0,0,4,0,
											0,4,0,0,
											0,0,0,1)),
								  glm::transpose(glm::mat4(0,-0.353f,-0.353f,0,
											0.5f,0,0,-2,
											0,-0.353f,0.353f,0,
											0,0,0,1))

								};
		for ( int i = 0; i < 4; i++)
		{
			addFigures(fName[i], figures, shaderTexture, texID[i], modelMat[i], (i == 0 ? 50 : 1) );
		}
	}



	Vector3D e(0,2,-20);
	Vector3D g(0,-0.1,1);
	Vector3D t(0,1,0);
	Camera* cam = new Perspective(e,g,t);
	glm::mat4 projMatrix = cam->getProjectionMatrix(10,-10,1);
	glm::mat4 viewMatrix = cam->getViewMatrix();

	// Loop until the user closes the window 
	double xposC, xposP, yposC, yposP;
	while (!glfwWindowShouldClose(window))
	{
		// Clear the window's buffer (or clear the screen to our
		// background color)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//
		// Eventually, any OpenGL code that renders your scene can go here
		// 

		renderAll(figures, projMatrix, viewMatrix);


		// Swap the front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process any events that may have occured with
		// the mouse or keyboard 
		glfwPollEvents();
		Vector3D change(0,0,0);
		if (glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, 1);
		if (glfwGetKey( window, GLFW_KEY_A) == GLFW_PRESS)
		{
			change[0] = -0.1;
		}
		if (glfwGetKey( window, GLFW_KEY_S) == GLFW_PRESS)
		{
			change[1] = -0.1;
		}
		if (glfwGetKey( window, GLFW_KEY_W) == GLFW_PRESS)
		{
			change[1] = 0.1;
		}
		if (glfwGetKey( window, GLFW_KEY_D) == GLFW_PRESS)
		{
			change[0] = 0.1;
		}
		if (glfwGetKey( window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			change[2] = -0.1;
		}
		if (glfwGetKey( window, GLFW_KEY_E) == GLFW_PRESS)
		{
			change[2] = 0.1;
		}

		glfwGetCursorPos(window, &xposC, &yposC);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			double difX = xposC - xposP;
			double difY = yposC - yposP;
			std::cout << difX << " " << difY << std::endl;
			if (difX != difY || difX != 0)
			cam->rotateGaze(difX, difY);
		}
		yposP = yposC;
		xposP = xposC;
		cam->updatePos(change);
		viewMatrix = cam->getViewMatrix();

	}
  
	// Terminate the application, close the window and clean-up any
	// OpenGL context memory
	glfwTerminate();
	
	exit(EXIT_SUCCESS);
}


void addFigures(std::string filename, std::vector<Figure*>* figures, sivelab::GLSLObject s, GLuint texID, glm::mat4 modelMatrix, int scale)
{

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


		// Store all triangles in the OBJ file in a triangle list
		// this is the primary list of all geometry in the 3D model
		// that we will build up while iterating over the structures
		// in the OBJ file data.
		std::vector<Triangle *> triangleList;

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


		Figure * fig = new Figure();
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

			Triangle * t = new Triangle();
			Vector3D color(1,0,0);
			Vector3D norm = (tv2 - tv0).cross(tv1 - tv0);
			t->glInit(tv0, norm, color, glm::vec2(0,0), tv1, norm, color, glm::vec2( (i%2) * scale,scale), tv2, norm, color, glm::vec2(scale,( (i + 1) %2) * scale), texID);

			// In case you're interested, the OBJ files also provide the
			// normal vectors at each vertex. You can access them like this:
			// 
			// v0.normal[0]
			// v0.normal[1]
			// v0.normal[2]
			// and so on for each vertex

			// be sure to add this triangle onto the triangle list
			triangleList.push_back(t);

		} 

		fig->glInit(triangleList, modelMatrix, s);
		figures->push_back(fig);
	}
}

void renderAll(std::vector<Figure*>* figures, glm::mat4 projMatrix, glm::mat4 viewMatrix)
{
	for (int i = 0; i < figures->size(); i++)
		(*figures)[i]->glRender(projMatrix, viewMatrix);
}
#endif