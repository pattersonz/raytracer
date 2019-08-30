#include "Model.h"
#include "BVA.h"	


png::rgb_pixel Model::getColor(const Vector3D e, const Vector3D intersect, const float tI, const std::vector<Light*> lights, const BVA* bva, int depth, Vector3D& norm) const
{

  png::rgb_pixel pix;
  if (! shade->getReflective() && !shade->getDielectric())
  	pix = png::rgb_pixel(0,0,0);
  else if (shade->getDielectric())
  	pix = getRefraction(e, intersect, tI, norm, lights, bva, depth);
  else
  	pix =  getNextColor(e, intersect, norm, lights, bva, depth);
  return shade->getColor(e, intersect, norm, lights, isPathToLight(intersect, lights, bva), pix);
}

Model * Model::getClosestHit(const Vector3D e, const Vector3D s, const BVA* bva, float& t, Vector3D& norm)  const
{
	Vector3D dir = s - e;

	Model * modelHit = 0;
	float length = dir.normalize();

	bva->checkCollisions(dir, e, 0.001f, t, modelHit, norm);

  	t *= length;

  	return modelHit;
}

std::vector<int> Model::isPathToLight(const Vector3D e, const std::vector<Light *> lights, const BVA* bva) const
{
	std::vector<int> hitLights;
	for (int i = 0; i < lights.size(); i++)
	{
		Vector3D s = lights[i]->getPos();

		float t = (s - e).length();
		Vector3D norm(1,0,0);
		Model * modelHit = getClosestHit(e, s, bva, t, norm);

	  	if (modelHit == 0)
	  		hitLights.push_back(i);
    }
    return hitLights;
}


png::rgb_pixel Model::getNextColor(const Vector3D e, const Vector3D intersect, const Vector3D normal, const std::vector<Light *>lights, const BVA* bva, int depth) const
{
	if (depth-- <= 0)
		return png::rgb_pixel(0,0,0);

	Vector3D d = intersect - e;
	d.normalize();
	Vector3D n = normal;
	n.normalize();
	Vector3D s = d - (2 * d.dot(n) * n);
	float t = 0.0f;
	Vector3D nextNorm(1,0,0);
	Model * nextHit = getClosestHit(intersect, intersect + s, bva, t, nextNorm);

	if (nextHit == 0)
		return png::rgb_pixel(0,0,0);

	return nextHit->getColor(intersect, intersect + t * (s), t, lights, bva, depth, nextNorm);
}

png::rgb_pixel Model::getRefraction(const Vector3D e, const Vector3D intersect, const float tI, const Vector3D normal, const std::vector<Light *>lights, const BVA* bva, int depth) const
{
	if (depth-- < 1)
		return png::rgb_pixel(0,0,0);


	Vector3D d = intersect - e;
	d.normalize();
	Vector3D dMinus = -1 * d;

	float nRI = 1.0f;
	Vector3D n = normal;
	n.normalize();

	float c;
	Vector3D k;
	Vector3D t;

	
	if (d.dot(n) < 0)
	{
		refract(d, n, nRI, shade->getRI(), t);
		t.normalize();
		c = dMinus.dot(n);
		k[0] = k[1] = k[2] = 1.0f;
	}
	else
	{
		k = Vector3D(1,1,1);
		/*k = shade->getAC();
		for (int i = 0; i < 3; i++)
			k[i] = exp(-1 * (k[i] - 1) * tI);*/
		float nRIT = nRI;
		nRI = shade->getRI();

		if (refract(d, -1 * n, nRI, nRIT, t))
		{
			t.normalize();
			c = t.dot(n);
		}
		else
		{
			png::rgb_pixel pix =  getNextColor(e, intersect, n, lights, bva, depth);
			pix.red *= k[0]; pix.green *= k[1]; pix.blue *= k[2];
			return pix;
		}
	}

	float R0 = pow(nRI - 1,2)/pow(nRI + 1,2);
	float R = R0 + (1 - R0)*pow( 1 - c,5);

	png::rgb_pixel pixNC = getNextColor(e, intersect, n, lights, bva, depth);
	Vector3D nextColorPix(pixNC.red, pixNC.green, pixNC.blue);

	pixNC = getRefractionColor(intersect, t, lights, bva, depth);
	Vector3D nextColorPixR(pixNC.red, pixNC.green, pixNC.blue);


	Vector3D pixV =       R * nextColorPix + 
						 (1 - R) * nextColorPixR;
	png::rgb_pixel pix(pixV[0], pixV[1], pixV[2]);
	pix.red *= k[0]; pix.green *= k[1]; pix.blue *= k[2];
	return pix;
}


bool Model::refract(const Vector3D d, const Vector3D n, const float nRI, const float nRIT, Vector3D& t) const
{
	float radVal;

	radVal = 1 - (nRI * nRI * (1.0f - pow(d.dot(n), 2))  / (nRIT * nRIT));

	if (radVal < 0)
		return false;

	t = ( nRI * (d - n * d.dot(n) ) / nRIT) - (n * sqrt(radVal));

	return true;
}

png::rgb_pixel Model::getRefractionColor(const Vector3D intersect, const Vector3D dir, const std::vector<Light *>lights, const BVA* bva, int depth) const
{
	Model* nextHit = 0;
	float t = 0.0f;
	Vector3D nNorm(1,0,0);

	bva->checkCollisions(dir, intersect, 0.001f, t, nextHit, nNorm);

	if (nextHit != 0)
    	return nextHit->getColor(intersect, intersect + t * dir, t, lights, bva, depth, nNorm);
    else
    {
    	return png::rgb_pixel(0,0,0);
    }

}