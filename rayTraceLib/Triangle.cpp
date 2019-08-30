#include "Triangle.h"

#define G d[0]
#define H d[1]
#define I d[2]

bool Triangle::getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& modelHit)
{
  float t, beta, gamma, M;
  float A,B,C,D,E,F,J,K,L;

  A = a[0] - b[0];   D = a[0] - c[0];   J = a[0] - e[0];
  B = a[1] - b[1];   E = a[1] - c[1];   K = a[1] - e[1];
  C = a[2] - b[2];   F = a[2] - c[2];   L = a[2] - e[2];
  
  float EIHF = (E * I - H * F);
  float GFDI = (G * F - D * I);
  float DHEG = (D * H - E * G);
  M = A * EIHF + B * GFDI + C * DHEG;

  float AKJB = (A * K - J * B);
  float JCAL = (J * C - A * L);
  float BLKC = (B * L - K * C);

  t = -1 * ( F * AKJB + E * JCAL + D * BLKC) / M;

  if ( ! (t > minT && (maxT > t || maxT == 0.0f)))
    return false;

  gamma = ( I * AKJB + H * JCAL + G * BLKC) / M;

  if (gamma < 0 || gamma > 1)
    return false;

  beta = (J * EIHF + K * GFDI + L * DHEG) / M;

  if ( beta < 0  || beta > 1 - gamma)
    return false;

  maxT = t;
  return true;
  
}

png::rgb_pixel Triangle::getNormalAsColor(Vector3D interP) const
{


	return png::rgb_pixel((norm[0] + 1.0f) / 2.0f * 255,
							  (norm[1] + 1.0f) / 2.0f * 255,
							  (norm[2] + 1.0f) / 2.0f * 255 );
}

png::rgb_pixel Triangle::getColor(const Vector3D e, const Vector3D intersect, const std::vector<Light*> lights, const std::vector<Model *> models, int depth) const
{
    if (! shade->getReflective())
  	return shade->getColor(e, intersect, norm, lights, isPathToLight(intersect, lights, models));
  else
  	return getNextColor(e, intersect, norm, lights, models, depth);
}
