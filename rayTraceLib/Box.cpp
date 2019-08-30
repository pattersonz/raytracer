#include "Box.h"

bool Box::getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& hitModel)
{
  for (int i = 0; i < 12; i++)
  {
  	if (tris[i]->getCollisions(d, e, minT, maxT, hitModel))
  	{
  		hitModel = tris[i];
  	}
  }
  return false;
  
}


png::rgb_pixel Box::getNormalAsColor(Vector3D interP) const
{
	return png::rgb_pixel(0,0,0);
}

png::rgb_pixel Box::getColor(const Vector3D e, const Vector3D intersect, const std::vector<Light*> lights, const std::vector<Model *> models, int depth) const
{
	return png::rgb_pixel(0,0,0);
}

