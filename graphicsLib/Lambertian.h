#ifndef _LAMBERTIANH_
#define _LAMBERTIANH_

#include "Shader.h"
#include "Vector3D.h"

using namespace sivelab;

class Lambertian : public Shader
{
  private:
  	Vector3D diffuse;
  public:
  	Lambertian (std::string name, Vector3D dif, bool isReflective)
  	{
  		this->name = name;
  		diffuse = dif;
  		this->isReflective = isReflective;
      isDielectric = false;
  	}

  	virtual png::rgb_pixel getColor(const Vector3D e, const Vector3D intersect, const Vector3D normal, const std::vector<Light*> lights, const std::vector<int> hitLights, png::rgb_pixel reflect) const;
    virtual png::rgb_pixel getFragmentColor(glm::vec4 normal, std::vector<glm::vec3> lightDirs, std::vector<glm::vec3> halves, std::vector<Light*> lights) const;
    
};

#endif