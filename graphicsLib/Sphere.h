#ifndef __SPHEREH__
#define __SPHEREH__
#include <cmath>
#include "Vector3D.h"
#include "png++/png.hpp"
#include "Model.h"
#include "Shader.h"
#include "Triangle.h"
#include "model_obj.h"
using namespace sivelab;

class Sphere : public Model
{
private:
	float radius;
public:
	Sphere(std::string name, Vector3D pos, float rad, Shader* s)
	{
		id = name;
		shade = s;
		position = pos;
		radius = rad;
	}

	virtual bool getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& modelHit, Vector3D& norm);

	virtual png::rgb_pixel getNormalAsColor(Vector3D interP) const;
	

	virtual std::vector<Triangle*> getListOfTris() const;

	virtual void getBoundaries(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax);

	virtual Model* deepCopy() const
	{
		Model* newM;
		newM = new Sphere(id, position, radius, shade);

		return newM;
	}
};

#endif
