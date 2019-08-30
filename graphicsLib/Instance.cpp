#include "Instance.h"

#ifndef GLM_SWIZZLE
#define GLM_SWIZZLE
#endif



bool Instance::getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& hitModel, Vector3D& norm)
{
  Vector3D d2, e2;
  double w = 0.0f;
  double w2 = 1.0f;
  d2 = m.multVector(d,w);
  e2 = m.multVector(e,w2);
  Model* temp = hitModel; 
  bool gotHit = model->getCollisions(d2, e2, minT, maxT, hitModel, norm);
  if (gotHit || temp != hitModel)
  {
    Matrix4x4 mIT = m.transpose();
    w = 0.0f;
    norm = mIT.multVector(norm, w);
    norm.normalize();
    return true;
  }
  return false;
}


png::rgb_pixel Instance::getNormalAsColor(Vector3D interP) const
{
  /*
  Vector3D i2;
  double w = 1.0f;
  i2 = m.multVector(interP,w);*/
  return model->getNormalAsColor(interP);
}


std::vector<Triangle*> Instance::getListOfTris() const
{

  return model->getListOfTris();
}


void Instance::getBoundaries(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax)
{
  model->getBoundaries(xmin, xmax, ymin, ymax, zmin, zmax);
  Matrix4x4 mI = m.inverse();

  float x[2] = {xmin, xmax};
  float y[2] = {ymin, ymax};
  float z[2] = {zmin, zmax};

  double w = 1.0f;

  Vector3D thisPoint(x[0], y[0], z[0]);
  thisPoint = mI.multVector(thisPoint, w);
  xmin = xmax = thisPoint[0];
  ymin = ymax = thisPoint[1];
  zmin = zmax = thisPoint[2];

  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++)
      for (int k = 0; k < 2; k++)
      {
        thisPoint = Vector3D(x[i], y[j], z[k]);
        thisPoint = mI.multVector(thisPoint, w);

        if (thisPoint[0] < xmin)
          xmin = thisPoint[0];
        if (thisPoint[0] > xmax)
          xmax = thisPoint[0];
        if (thisPoint[1] < ymin)
          ymin = thisPoint[1];
        if (thisPoint[1] > ymax)
          ymax = thisPoint[1];
        if (thisPoint[2] < zmin)
          zmin = thisPoint[2];
        if (thisPoint[2] > zmax)
          zmax = thisPoint[2];
      }
}