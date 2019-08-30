#ifndef __INSTANCEH__
#define __INSTANCEH__

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define GLM_FORCE_RADIANS

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLSL.h"


#include <cmath>
#include "Matrix4x4.h"
#include "Vector3D.h"
#include "png++/png.hpp"
#include "Model.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Box.h"
#include "Shader.h"
#include "Figure.h"
#include "model_obj.h"


using namespace sivelab;

class Instance : public Model
{
private:
	Model* model;
	Matrix4x4 m;


public:
	Instance()
	{
		
	}

	Instance(std::string name, Matrix4x4 m, Model* model, Shader* s)
	{
		id = name;
		shade = s;
		this->model = model;
		this->m = m.inverse();
	}

	Instance(std::string name, Matrix4x4 m, Model* model)
	{
		id = name;
		shade = model->getShader();
		this->model = model;
		this->m = m.inverse();
	}


	virtual bool getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& modelHit, Vector3D& norm);

	virtual png::rgb_pixel getNormalAsColor(Vector3D interP) const;


	virtual std::vector<Triangle*> getListOfTris() const;

	virtual void getBoundaries(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax);

	virtual Model* deepCopy() const
	{
		Model* newM;
		newM = model->deepCopy();
		Model* newInstance;
		newInstance = new Instance(id, m.inverse(), newM, shade);

		return newInstance;
	}


};

#endif
