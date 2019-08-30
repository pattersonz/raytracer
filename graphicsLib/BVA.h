#ifndef __BVAH__
#define __BVAH__

#define GETMIN(x,y) ( (x) < (y) ? (x) : (y))
#define GETMAX(x,y) ( (x) > (y) ? (x) : (y))

#include <vector>
#include "Model.h"
#include "Vector3D.h"
#include "Shader.h"
#include <stdio.h>
#include <limits>

class BVA
{
private:
	BVA* leftBox;
	BVA* rightBox;

	bool isLeaf;
	Model* model;


	static void mergeSort(std::vector<BVA *>& list, const int type);

public:
	float xmin, xmax, ymin, ymax, zmin, zmax;
	BVA(BVA* l, BVA* r)
	{
		leftBox = l;
		rightBox = r;
		if (r != 0)
		{
			xmin = GETMIN(l->xmin, r->xmin);
			xmax = GETMAX(l->xmax, r->xmax);
			ymin = GETMIN(l->ymin, r->ymin);
			ymax = GETMAX(l->ymax, r->ymax);
			zmin = GETMIN(l->zmin, r->zmin);
			zmax = GETMAX(l->zmax, r->zmax);
		}
		else
		{
			xmin = l->xmin;
			xmax = l->xmax;
			ymin = l->ymin;
			ymax = l->ymax;
			zmin = l->zmin;
			zmax = l->zmax;
		}

		isLeaf = false;
		model = 0;
	}

	BVA(Model* m)
	{
		model = m;
		isLeaf = true;

		m->getBoundaries(xmin, xmax, ymin, ymax, zmin, zmax);

		leftBox = rightBox = 0;
	}

	BVA(std::vector<BVA *> m, int height);

	/*
	checks to see if the given ray intersects with models in the volume
	*/
	bool checkCollisions(const Vector3D dir, const Vector3D e, const float minT,float& maxT, Model*& modelHit, Vector3D& norm) const;

	/*
	checks to see if the ray intersects with the volume we are encasing
	*/
	bool rayHitsThisVol(const Vector3D dir, const Vector3D e, const float minT) const;

	/*
	method that creates a BVA structure from a vector of models
	*/
	static BVA* createBVA(const std::vector<Model *> models);

	void updateLeafShaders(Shader * newS);

};


#endif