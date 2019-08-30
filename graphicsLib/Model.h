#ifndef __MODELH__
#define __MODELH__
#include "Vector3D.h"
#include "png++/png.hpp"
#include "Shader.h"
#include "model_obj.h"
#include <math.h>
using namespace sivelab;

class Triangle;
class BVA;

class Model
{
protected:
	Shader * shade;
	Vector3D position;
	std::string id;

	/*
	calculates the refraction ray. If there is no refraction, returns false
	*/
	bool refract(const Vector3D d, const Vector3D n, const float nRI, const float nRIT,  Vector3D& t) const;

	png::rgb_pixel getRefractionColor(const Vector3D intersect, const Vector3D dir, const std::vector<Light *>lights, const BVA* bva, int depth) const;

public:
	Model() {}
	

	/*
	Calculates intersection between the model and the ray bneing cast. The function
	returns true if there is a closer intersection. If the intersection point is closer
	to the camera than the previous closest, that value is used instead. 
	*/
	virtual bool getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& modelHit, Vector3D& norm) = 0;

	/*
	  returns the color of the model
	 */
	png::rgb_pixel getColor(const Vector3D e, const Vector3D intersect, const float tI, const std::vector<Light*> lights, const BVA* bva, int depth, Vector3D& norm) const;

	/*
	returns the color of the normal from the center
	*/
	virtual png::rgb_pixel getNormalAsColor(Vector3D interP) const = 0;

	/*
	returns the shader for the Model
	*/
	Shader* getShader()
	{return shade;}

	/*
	Gets the closest object
	*/
	Model * getClosestHit(const Vector3D e, const Vector3D s, const BVA* bva, float& t, Vector3D& norm)  const;


	/*
	Checks to see if there exists a direct path to a light
	*/
	std::vector<int> isPathToLight(const Vector3D e, const std::vector<Light *> lights, const BVA* bva) const;


	/*
	Given the normal of the surface hit, point of intersection and the origin of the initial ray
	this returns the color of the next object providing this was reflected off the surface.
	*/
	png::rgb_pixel getNextColor(const Vector3D e, const Vector3D intersect, const Vector3D normal, const std::vector<Light *>lights, const BVA* bva, int depth) const;

	/*
	Works similarly to getNextColor. Although instead of simply reflecting the ray, we need to potentially bend it through our model.
	*/
	png::rgb_pixel getRefraction(const Vector3D e, const Vector3D intersect, const float tI, const Vector3D normal, const std::vector<Light *>lights, const BVA* bva, int depth) const;


	virtual std::vector<Triangle*> getListOfTris() const = 0;


	/*
	takes in 6 out parameters which designate a box that encases the model object. These values are
	set to the furthest points in each respective direction
	*/
	virtual void getBoundaries(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax) = 0;

	/*
	returns the id
	*/
	std::string getID()
	{
		return id;
	}

	/*
	changes the shader for the model, if this model has submodels, those are updated too
	*/
	virtual void updateShader(Shader * newS)
	{
		shade = newS;
	}

	/*
	returns a copy of the model that does not share pointers to any other model
	*/
	virtual Model* deepCopy() const = 0;


};
#endif
