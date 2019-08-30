#ifndef __TRIANGLEH__
#define __TRIANGLEH__

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLSL.h"

#include <cmath>
#include "Vector3D.h"
/*#include "png++/png.hpp"
#include "Model.h"
#include "Shader.h"*/

using namespace sivelab;

class Triangle
{
private:
	Vector3D a, b, c;
	Vector3D norm;
	GLuint m_VAO;
	sivelab::GLSLObject shader;
	GLint pMatID;
public:
	Triangle()
	{
		a = b = c = Vector3D(0,0,0);
	}

	Triangle(Vector3D a, Vector3D b, Vector3D c/*, Shader* s*/)
	{
		//shade = s;
		this->a = a;
		this->b = b;
		this->c = c;
		Vector3D oneTwo = b - a, oneThree = c - a;
		norm = oneTwo.cross(oneThree);
		norm.normalize();
	}


	//virtual bool getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& modelHit);

	//virtual png::rgb_pixel getNormalAsColor(Vector3D interP) const;

	//virtual png::rgb_pixel getColor(const Vector3D e, const Vector3D intersect, const std::vector<Light*> lights, const std::vector<Model *> models, int depth) const;

	/*
	This function initializes a triangle for use with openGL. This is
	achieved by taking in three vectors to represent the three verticies
	and another vector for each representing a color for the vertex.
	*/
	void glInit(Vector3D a, Vector3D aN, Vector3D aC, Vector3D b, Vector3D bN,
  Vector3D bC, Vector3D c, Vector3D cN, Vector3D cC, sivelab::GLSLObject shaderN);

	/*
	This function runs the code to render the triangle on the screen
	*/
	void glRender(glm::mat4 projMatrix);


};

#endif
