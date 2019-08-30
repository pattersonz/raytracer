#ifndef __BOXH__
#define __BOXH__
#include <cmath>
#include "Vector3D.h"
#include "png++/png.hpp"
#include "Model.h"
#include "Triangle.h"
#include "Shader.h"
#include "model_obj.h"
using namespace sivelab;

class Box : public Model
{
private:
	Triangle* tris[12];
	Vector3D minP, maxP;
public:
	Box(std::string name, Vector3D minP, Vector3D maxP, Shader* s)
	{
		this->minP = minP;
		this->maxP = maxP;
		id = name;
		shade = s;
		position = (minP + maxP) * (1.0f / 2.0f);
		Vector3D points[8];
		points[0] = minP;
		points[1] = Vector3D(minP[0], minP[1], maxP[2]);
		points[2] = Vector3D(minP[0], maxP[1], minP[2]);
		points[3] = Vector3D(minP[0], maxP[1], maxP[2]);
		points[4] = Vector3D(maxP[0], minP[1], minP[2]);
		points[5] = Vector3D(maxP[0], minP[1], maxP[2]);
		points[6] = Vector3D(maxP[0], maxP[1], minP[2]);
		points[7] = maxP;
		tris[0] = new Triangle(name, points[0], points[1], points[2], s);
		tris[1] = new Triangle(name, points[1], points[3], points[2], s);
		tris[2] = new Triangle(name, points[0], points[2], points[4], s); 
		tris[3] = new Triangle(name, points[2], points[6], points[4], s); 
		tris[4] = new Triangle(name, points[0], points[4], points[1], s);
		tris[5] = new Triangle(name, points[1], points[4], points[5], s);
		tris[6] = new Triangle(name, points[4], points[6], points[5], s); 
		tris[7] = new Triangle(name, points[5], points[6], points[7], s); 
		tris[8] = new Triangle(name, points[2], points[3], points[6], s);
		tris[9] = new Triangle(name, points[6], points[3], points[7], s);
		tris[10] = new Triangle(name, points[1], points[5], points[3], s);
		tris[11] = new Triangle(name, points[5], points[7], points[3], s);
		
	}

	virtual bool getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& modelHit, Vector3D& norm);

	virtual png::rgb_pixel getNormalAsColor(Vector3D interP) const;

	virtual std::vector<Triangle*> getListOfTris() const;

	virtual void getBoundaries(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax);

	virtual void updateShader(Shader * newS);

	virtual Model* deepCopy() const
	{
		Model* newM;
		newM = new Box(id, minP, maxP, shade);

		return newM;
	}
};

#endif
