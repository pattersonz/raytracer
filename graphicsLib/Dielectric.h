#ifndef _DIELECTRICH_
#define _DIELECTRICH_

#include "Shader.h"
#include "Vector3D.h"

using namespace sivelab;

class Dielectric : public Shader
{
  private:
  public:
  	Dielectric (std::string name, float rI, Vector3D aC)
  	{
  		this->name = name;
      refractiveIndex = rI;
      attenuationCoef = aC;
  		this->isReflective = false;
  		this->isDielectric = true;
  	}

  	virtual png::rgb_pixel getColor(const Vector3D e, const Vector3D intersect, const Vector3D normal, const std::vector<Light*> lights, const std::vector<int> hitLights, png::rgb_pixel reflect) const;
  	virtual png::rgb_pixel getFragmentColor(glm::vec4 normal, std::vector<glm::vec3> lightDirs, std::vector<glm::vec3> halves, std::vector<Light*> lights) const;
};

#endif
