#ifndef __SPHEREH__
#define __SPHEREH__
#include <cmath>
#include "Vector3D.h"
#include "png++/png.hpp"
#include "Model.h"
#include "Shader.h"
using namespace sivelab;

class Sphere : public Model
{
private:
	float radius;
public:
	Sphere(Vector3D pos, float rad, Shader* s)
	{
		shade = s;
		position = pos;
		radius = rad;
	}

	virtual bool getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& modelHit);

	virtual png::rgb_pixel getNormalAsColor(Vector3D interP) const;
	
	virtual png::rgb_pixel getColor(const Vector3D e, const Vector3D intersect, const std::vector<Light*> lights, const std::vector<Model *> models, int depth) const;


};

#endif
