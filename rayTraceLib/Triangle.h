#ifndef __TRIANGLEH__
#define __TRIANGLEH__
#include <cmath>
#include "Vector3D.h"
#include "png++/png.hpp"
#include "Model.h"
#include "Shader.h"

using namespace sivelab;

class Triangle : public Model
{
private:
	Vector3D a, b, c;
	Vector3D norm;
public:
	Triangle()
	{
		a = b = c = Vector3D(0,0,0);
	}

	Triangle(Vector3D a, Vector3D b, Vector3D c, Shader* s)
	{
		shade = s;
		position = (a + b + c) * (1.0f / 3.0f);
		this->a = a;
		this->b = b;
		this->c = c;
		Vector3D oneTwo = b - a, oneThree = c - a;
		norm = oneTwo.cross(oneThree);
		norm.normalize();
	}


	virtual bool getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& modelHit);

	virtual png::rgb_pixel getNormalAsColor(Vector3D interP) const;

	virtual png::rgb_pixel getColor(const Vector3D e, const Vector3D intersect, const std::vector<Light*> lights, const std::vector<Model *> models, int depth) const;




};

#endif
