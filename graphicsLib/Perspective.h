#ifndef __PERSPECTIVEH__
#define __PERSPECTIVEH__

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cstdio>
#include <ctime>
#include <cmath>
#include <vector>
#include <stdlib.h>  
#include <time.h> 
#include "Vector3D.h"
#include "Camera.h"
#include "BVA.h"
#include "Model.h"
#include "Figure.h"
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

  	Perspective(Vector3D e, Vector3D g, Vector3D t)
  	{
  		pos = e;
      setDirection(g / g.length());
  	}
  	
  	virtual glm::mat4 getProjectionMatrix(float r, float t, float n) const;


	virtual void rayTraceScene(png::image < png::basic_rgb_pixel <unsigned char> > & screen, const std::vector<Model *> models, const std::vector<Light *> lights) const;

	virtual void rasterizeScene(png::image < png::basic_rgb_pixel <unsigned char> > & screen, const std::vector<Model *> models, const std::vector<Light *> lights) const;

	glm::mat4 getMperspective(const float r, const float t, const float n) const;
	
};

#endif
