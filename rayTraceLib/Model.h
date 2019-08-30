#ifndef __MODELH__
#define __MODELH__
#include "Vector3D.h"
#include "png++/png.hpp"
#include "Shader.h"

using namespace sivelab;

class Model
{
protected:
	Shader * shade;
	Vector3D position;
public:
	Model() {}
	
	/*
	Calculates intersection between the model and the ray bneing cast. The function
	returns true if there is a closer intersection. If the intersection point is closer
	to the camera than the previous closest, that value is used instead. 
	*/
	virtual bool getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& modelHit) = 0;

	/*
	  returns the color of the model
	 */
	virtual png::rgb_pixel getColor(const Vector3D e, const Vector3D intersect, const std::vector<Light*> lights, const std::vector<Model *> models, int depth) const = 0;

	/*
	returns the color of the normal from the center
	*/
	virtual png::rgb_pixel getNormalAsColor(Vector3D interP) const = 0;


	/*
	Gets the closest object
	*/
	Model * getClosestHit(const Vector3D e, const Vector3D s, const std::vector<Model *> models, float& t)  const
	{
		Vector3D dir = s - e;

		Model * modelHit = NULL;
		float length = dir.normalize();


		int modelsInScene = models.size();
		for (int k = 0; k < modelsInScene; k++)
	  	{
			if ( models[k]->getCollisions(dir, e, 0.001f, t, modelHit))
				modelHit = models[k];
	  	}
	  	t *= length;
	  	return modelHit;
	}

	/*
	Checks to see if there exists a direct path to a light
	*/
	std::vector<int> isPathToLight(const Vector3D e, const std::vector<Light *> lights, const std::vector<Model *> models) const
	{
		std::vector<int> hitLights;
		for (int i = 0; i < lights.size(); i++)
		{
			Vector3D s = lights[i]->getPos();

			float t = (s - e).length();
			Model * modelHit = getClosestHit(e, s, models, t);

		  	if (modelHit == NULL)
		  		hitLights.push_back(i);
        }
        return hitLights;
	}

	/*
	Given the normal of the surface hit, point of intersection and the origin of the initial ray
	this returns the color of the next object providing this was reflected off the surface.
	*/
	png::rgb_pixel getNextColor(const Vector3D e, const Vector3D intersect, const Vector3D normal, const std::vector<Light *>lights, const std::vector<Model *> models, int depth) const
	{
		if (depth-- <= 0)
			return png::rgb_pixel(0,0,0);

		Vector3D d = intersect - e;
		d.normalize();
		Vector3D s = d - 2 * (d.dot(normal)) * normal;
		float t = 0.0f;
		Model * nextHit = getClosestHit(intersect, intersect + s, models, t);

		if (nextHit == NULL)
			return png::rgb_pixel(0,0,0);

		return nextHit->getColor(intersect, intersect + t * (s), lights, models, depth);
	}


};
#endif
