#ifndef __PERSPECTIVEH__
#define __PERSPECTIVEH__

#include <vector>
#include "Vector3D.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "png++/png.hpp"

using namespace sivelab;
class Perspective : public Camera
{
  public:
  	Perspective()
  	{
		pos = Vector3D(0,0,0);
		u = Vector3D(1,0,0);
		v = Vector3D(0,1,0);
		w = Vector3D(0,0,-1);
		d = 1.0f;
		imagePlaneX = 1.0f;
		imagePlaneY = 1.0f;
		imagePixelX = 100;
		imagePixelY = 100;
		depth = 4;
  	}

	virtual void drawScene(png::image < png::basic_rgb_pixel <unsigned char> > & screen, const std::vector<Model *> models, const std::vector<Light *> lights) const;


};

#endif