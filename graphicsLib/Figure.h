#ifndef __FIGUREH__
#define __FIGUREH__

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
#include "Triangle.h"
#include "Sphere.h"
#include "Box.h"
#include "Shader.h"
#include "model_obj.h"
#include "BVA.h"

using namespace sivelab;

class Figure : public Model
{
private:
	std::vector<Triangle *> tris;
	glm::mat4 modelMatrix;
	sivelab::GLSLObject shader;
	BVA* bva;
	/*
	Returns the lowest value of a, b, or c provided it is greater than d, else return d 
	*/
	int getFloorIn(float a, float b, float c, int d) const;
	/*
	Returns the largest value of a, b, or c provided it is less than d, else return d 
	*/
	int getRoofIn(float a, float b, float c, int d) const;
	
public:
	Figure()
	{
		
	}

	Figure(std::string name, std::vector<Triangle *> newList, Shader* s)
	{
		id = name;
		shade = s;
		tris = newList;
		modelMatrix = glm::mat4(1,0,0,0,
								0,1,0,0,
								0,0,1,0,
								0,0,0,1);
		std::vector<Model *> m;
		for (int i = 0; i < newList.size(); i++)
			m.push_back(newList[i]);
		bva = BVA::createBVA(m);
	}

	Figure(std::string name, std::vector<Triangle *> newList, Shader* s, glm::mat4 newModelMatrix)
	{
		id = name;
		shade = s;
		tris = newList;
		modelMatrix = newModelMatrix;
	}

	virtual bool getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& modelHit, Vector3D& norm);

	virtual png::rgb_pixel getNormalAsColor(Vector3D interP) const;


	void glInit(std::vector<Triangle *> trisN, glm::mat4 modelMatrixN, sivelab::GLSLObject shaderN);

	void glRender(glm::mat4 projMatrix, glm::mat4 viewMatrix);

	virtual std::vector<Triangle*> getListOfTris() const;

	/*
	Returns the model matrix for this figure
	*/
	glm::mat4 getModelMatrix();

	/*
	updates the pixel matrix with this figure being rasterized
	*/
	void rasterizeFigure(png::image < png::basic_rgb_pixel <unsigned char> > & screen, const Vector3D camPos, const std::vector<Light *> lights, const glm::mat4 M, const glm::mat4 Mview, const glm::mat4 Mproj, const glm::mat4 Mvp, float* zBuf, const int imagePixelX, const int  imagePixelY) const;

	virtual void getBoundaries(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax);

	virtual void updateShader(Shader * newS);


	virtual Model* deepCopy() const
	{
		std::vector<Triangle *> newTriList;
		for (int i = 0; i < tris.size(); i++)
			newTriList.push_back((Triangle*)tris[i]->deepCopy());

		Model* newM;
		newM = new Figure(id, newTriList, shade);

		return newM;
	}
};

#endif
