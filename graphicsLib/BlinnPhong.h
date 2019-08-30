#ifndef _BLINNPHONGH_
#define _BLINNPHONGH_

#include "Shader.h"
#include "Vector3D.h"
#include <cmath>

using namespace sivelab;

class BlinnPhong : public Shader
{
  private:
  	Vector3D diffuse;
    Vector3D specular;
    float phongEXP;
  public:
  	BlinnPhong (std::string name, Vector3D dif, Vector3D spec, float phong, bool isReflective)
  	{
  		this->name = name;
  		diffuse = dif;
  		this->isReflective = isReflective;
      specular = spec;
      phongEXP = phong;
      isDielectric = false;
  	}

  	virtual png::rgb_pixel getColor(const Vector3D e, const Vector3D intersect, const Vector3D normal, const std::vector<Light*> lights, const std::vector<int> hitLights, png::rgb_pixel reflect) const;
    virtual png::rgb_pixel getFragmentColor(glm::vec4 normal, std::vector<glm::vec3> lightDirs, std::vector<glm::vec3> halves, std::vector<Light*> lights) const;

};

#endif
