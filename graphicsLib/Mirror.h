#ifndef _MIRRORH_
#define _MIRRORH_

#include "Shader.h"
#include "Vector3D.h"

using namespace sivelab;

class Mirror : public Shader
{
  private:
  public:
  	Mirror (std::string name, bool isReflective)
  	{
  		this->name = name;
  		this->isReflective = isReflective;
  		isDielectric = false;
  	}

  	virtual png::rgb_pixel getColor(const Vector3D e, const Vector3D intersect, const Vector3D normal, const std::vector<Light*> lights, const std::vector<int> hitLights, png::rgb_pixel reflect) const;
  	virtual png::rgb_pixel getFragmentColor(glm::vec4 normal, std::vector<glm::vec3> lightDirs, std::vector<glm::vec3> halves, std::vector<Light*> lights) const;
};

#endif
