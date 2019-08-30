#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Triangle.h"
#include "Vector3D.h"

#include "GLSL.h"

using namespace sivelab;


void glfwErrorCallback(int error, const char* description)
{
	std::cerr << "GLFW ERROR: " << description << std::endl;
}

/**
 * main entry function - Establishes the OpenGL context with the GLFW
 * (http://www.glfw.org/) and creates a window within this context.
 *
 * 
 */
int main(void)
{
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
	int winWidth = 800;
	float aspectRatio = 9.0 / 16.0; // winWidth / (float)winHeight;
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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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



	//T1
	Triangle t1;
	Vector3D a(-4.5f, -5.0f, 0.0f), b(0.0f, 5.0f, 0.0f), c(4.5f, -5.0f, 0.0f);
	Vector3D aN(0.0f, 0.0f, 1.0f), bN(0.0f, 0.0f, 1.0f), cN(0.0f, 0.0f, 1.0f);
	Vector3D aC(1.0f, 1.0f, 0.0f), bC(1.0f, 0.0f, 0.0f), cC(1.0f, 0.0f, 1.0f);

	t1.glInit(a,aN, aC,b,bN, bC,c,cN, cC, shaderLambertian);


	//T2
/*
	Triangle t2;
	
	a = Vector3D(-1.0f, -1.0f, 0.0f);b = Vector3D(1.0f, 1.0f, 0.0f); c = Vector3D(1.0f, -1.0f, 0.0f);
	Vector3D aN2(0.0f, 1.0f, 1.0f), bN2(1.0f, 0.0f, 1.0f), cN2(1.0f, 1.0f, 0.0f);
	
	t2.glInit(a,aN,b,cN,c,cN2);
*/





	glm::mat4 projMatrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -10.0f, 10.0f);

	// Loop until the user closes the window 
	while (!glfwWindowShouldClose(window))
	{
		// Clear the window's buffer (or clear the screen to our
		// background color)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//
		// Eventually, any OpenGL code that renders your scene can go here
		// 

		t1.glRender(projMatrix);
		//t2.glRender();



		// Swap the front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process any events that may have occured with
		// the mouse or keyboard 
		glfwPollEvents();
		if (glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, 1);
	}
  
	// Terminate the application, close the window and clean-up any
	// OpenGL context memory
	glfwTerminate();
	
	exit(EXIT_SUCCESS);
}

