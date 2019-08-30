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
#include "png++/png.hpp"
#include "Model.h"
#include "Shader.h"

#include "model_obj.h"

using namespace sivelab;

class Triangle : public Model
{
private:
	Vector3D a, b, c;
	ModelOBJ::Vertex aV, bV, cV;
	Vector3D norm;
	GLuint m_VAO;
	GLint pMatID;
	GLuint texID;
public:
	Triangle()
	{
		a = b = c = Vector3D(0,0,0);
		id =  "";
	}

	Triangle(std::string name, Vector3D a, Vector3D b, Vector3D c, Shader* s)
	{
		id = name;
		shade = s;
		position = (a + b + c) * (1.0f / 3.0f);
		this->a = a;
		this->b = b;
		this->c = c;
		Vector3D oneTwo = b - a, oneThree = c - a;
		norm = oneTwo.cross(oneThree);
		norm.normalize();

		for (int i = 0; i < 3; i++)
		{
			aV.position[i] = a[i];
			aV.normal[i] = norm[i];
			bV.position[i] = b[i];
			bV.normal[i] = norm[i];
			cV.position[i] = c[i];
			cV.normal[i] = norm[i]; 
		}
	}

		Triangle(std::string name, ModelOBJ::Vertex a, ModelOBJ::Vertex b, ModelOBJ::Vertex c, Shader* s)
	{
		id = name;
		shade = s;
		aV = a;
		bV = b;
		cV = c;


		for (int i = 0; i < 3; i++)
		{
			this->a[i] = aV.position[i];
			this->b[i] = bV.position[i];
			this->c[i] = cV.position[i];
		}

		position = (this->a + this->b + this->c) * (1.0f / 3.0f);
		norm = Vector3D(0,0,0);
		norm += Vector3D(a.normal[0], a.normal[1], a.normal[2]) / 3.0f;
		norm += Vector3D(b.normal[0], b.normal[1], b.normal[2]) / 3.0f;
		norm += Vector3D(c.normal[0], c.normal[1], c.normal[2]) / 3.0f;
		norm.normalize();
	}


	virtual bool getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& modelHit, Vector3D& norm);

	virtual png::rgb_pixel getNormalAsColor(Vector3D interP) const;

	/*
	This function initializes a triangle for use with openGL. This is
	achieved by taking in three vectors to represent the three verticies
	and another vector for each representing a color for the vertex.
	*/
	void glInit(Vector3D a, Vector3D aN, Vector3D aC, glm::vec2 aCoord, Vector3D b, Vector3D bN,
  		Vector3D bC, glm::vec2 bCoord, Vector3D c, Vector3D cN, Vector3D cC, glm::vec2 cCoord, const GLuint textureID = 0);

	/*
	This function runs the code to render the triangle on the screen
	*/
	void glRender(sivelab::GLSLObject shader);

	virtual std::vector<Triangle*> getListOfTris() const;

	/*
	returns the normal
	*/
	Vector3D getNormal();

	/*
	returns an array of the three vector3D
	*/
	void getVerticies(Vector3D*& tV);

	/*
	returns an array of three verticies
	*/
	void getVerticies(ModelOBJ::Vertex*& tV);

	virtual void getBoundaries(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax);

	virtual Model* deepCopy() const
	{
		Model* newM;
		newM = new Triangle(id, aV, bV, cV, shade);

		return newM;
	}

};

#endif
