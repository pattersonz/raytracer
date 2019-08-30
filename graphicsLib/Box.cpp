#include "Box.h"

bool Box::getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& hitModel, Vector3D& norm)
{
  Model* temp = hitModel;
  for (int i = 0; i < 12; i++)
  {
  	if (tris[i]->getCollisions(d, e, minT, maxT, hitModel, norm))
  	{
  		hitModel = tris[i];
  	}
  }

  return temp != hitModel;
  
}


png::rgb_pixel Box::getNormalAsColor(Vector3D interP) const
{
	return png::rgb_pixel(0,0,0);
}


std::vector<Triangle*> Box::getListOfTris() const
{
  std::vector<Triangle*> v;
  for (int i = 0; i < 12; i++)
    v.push_back(tris[i]);
  return v;
}

void Box::getBoundaries(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax)
  {
    float dummy1, dummy2, dummy3;
    tris[0]->getBoundaries(xmin, dummy1, ymin, dummy2, zmin, dummy3);
    tris[11]->getBoundaries(dummy1, xmax, dummy2, ymax, dummy3, zmax);
  }

void Box::updateShader(Shader * newS)
{
  shade = newS;
  for (int i = 0; i < 12; i++)
    tris[i]->updateShader(newS);
}
