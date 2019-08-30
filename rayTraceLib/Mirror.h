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
  	}

  	virtual png::rgb_pixel getColor(const Vector3D e, const Vector3D intersect, const Vector3D normal, const std::vector<Light*> lights, const std::vector<int> hitLights) const;
};

#endif
