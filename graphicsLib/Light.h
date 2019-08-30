#ifndef __LIGHTH__
#define __LIGHTH__

#include "Vector3D.h"
using namespace sivelab;

class Light
{
 protected:
  Vector3D position;
  Vector3D intensity;
public:
	Light() {}
	
	/*
	returns the position of the light
	*/
	Vector3D getPos() const {return position;}
	
	/*
	returns the intensity of the light
	*/
	Vector3D getIntensity() const {return intensity;}


};
#endif
