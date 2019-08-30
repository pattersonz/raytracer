#include <cstdlib>
#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
    glClearColor(0.0, 0.7, 1.0, 1.0);

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        // Clear the window's buffer (or clear the screen to our
        // background color)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //
        // Eventually, any OpenGL code that renders your scene can go here
        // 

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

