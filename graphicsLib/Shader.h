#ifndef _SHADERH_
#define _SHADERH_

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLSL.h"


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
	bool isDielectric;
	float amountReflect;
	float refractiveIndex;
    Vector3D attenuationCoef;
 public:
  Shader() {}
  std::string getName() {return name;}
  virtual png::rgb_pixel getColor(const Vector3D e, const Vector3D intersect, const Vector3D normal, const std::vector<Light*> lights, const std::vector<int> hitLights, png::rgb_pixel reflect) const = 0;
  bool getReflective() {return isReflective;}
  bool getDielectric() {return isDielectric;}
  virtual png::rgb_pixel getFragmentColor(glm::vec4 normal, std::vector<glm::vec3> lightDirs, std::vector<glm::vec3> halves, std::vector<Light*> lights) const = 0;

  float getRI() {return refractiveIndex;}
  Vector3D getAC() {return attenuationCoef;}
};

#endif
