#include "Sphere.h"

bool Sphere::getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& modelHit)
{
  float t;
  float A, B, C;
  A = d.dot(d);
  B = d.dot(e - position);
  C = (e-position).dot(e-position) - radius * radius;
  float disc = B * B  -  A * C;
  if (disc < 0)
    return false;
  t= (-1 * B - sqrt(disc)) / (A);
  if ( t > minT && (maxT > t || maxT == 0.0f))
  {
  	maxT = t;
  	return true;
  }
  return false;
  
}

png::rgb_pixel Sphere::getNormalAsColor(Vector3D interP) const
{
		interP -= position;
		interP.normalize();
		return png::rgb_pixel((interP[0] + 1.0f) / 2.0f * 255,
							  (interP[1] + 1.0f) / 2.0f * 255,
							  (interP[2] + 1.0f) / 2.0f * 255 );
}

png::rgb_pixel Sphere::getColor(const Vector3D e, const Vector3D intersect, const std::vector<Light*> lights, const std::vector<Model *> models, int depth) const
{
  Vector3D interP(intersect);
  interP -= position;
  interP.normalize();
  if (! shade->getReflective())
  	return shade->getColor(e, intersect, interP, lights, isPathToLight(intersect, lights, models));
  else
  	return getNextColor(e, intersect, interP, lights, models, depth);
}