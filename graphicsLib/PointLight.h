#ifndef __POINTLIGHTH__
#define __POINTLIGHTH__

#include "Light.h"
#include "Vector3D.h"
using namespace sivelab;

class PointLight: public Light
{
public:
	PointLight(Vector3D pos, Vector3D intens) 
	{
		position = pos;
		intensity = intens;
	}
};
#endif
