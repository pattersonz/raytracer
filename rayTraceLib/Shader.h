#ifndef _SHADERH_
#define _SHADERH_

#include <vector>
#include "png++/png.hpp"
#include "Vector3D.h"
#include "Light.h"

using namespace sivelab;

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define CLAMP(x,y,z) ((y) < (x) ? (x) : ((y) > (z) ? (z) : (y)) )

class Shader
{
protected:
	std::string name;
	bool isReflective;
 public:
  Shader() {}
  std::string getName() {return name;}
  virtual png::rgb_pixel getColor(const Vector3D e, const Vector3D intersect, const Vector3D normal, const std::vector<Light*> lights, const std::vector<int> hitLights) const = 0;
  bool getReflective() {return isReflective;}
};

#endif
