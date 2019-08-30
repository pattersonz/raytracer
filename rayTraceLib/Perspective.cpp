#include "Perspective.h"

void Perspective::drawScene(png::image < png::basic_rgb_pixel <unsigned char> > & screen, const std::vector<Model *> models, const std::vector<Light *> lights) const
  {

  	  	//location of camera
  	Vector3D e = pos;

  	//location of pixel
  	float pixelDimX = imagePlaneX / (float)imagePixelX;
  	float pixelDimY = imagePlaneY / (float)imagePixelY;

    Vector3D s;
  	//get color at each pixel
  	for (int i = 0; i < imagePixelX; i++)
  		for (int j = 0; j < imagePixelY; j++)
  		{

  			float offsetX = (((float)i + 0.5f) * pixelDimX) - imagePlaneX / 2.0f;
    		float offsetY = (((float)j + 0.5f) * pixelDimY) - imagePlaneY / 2.0f;

  			// origin + (d * -w) + offsetX * u + offsetY * v
		    s[0] = e[0] + d * -1 * w[0] + offsetX * u[0] + offsetY * v[0];
		    s[1] = e[1] + d * -1 * w[1] + offsetX * u[1] + offsetY * v[1];
		    s[2] = e[2] + d * -1 * w[2] + offsetX * u[2] + offsetY * v[2];

		    float t = 0.0f;

		      	//direction of the ray
		  	Vector3D dir = s - e;

		  	int modelsInScene = models.size();
		    Model * modelHit = NULL;
		  	//for all models in the scene, if the array collides a closer space, update the color
		  	for (int k = 0; k < modelsInScene; k++)
		  		if (models[k]->getCollisions(dir, e, 1.0f, t, modelHit))
            modelHit = models[k];

			  
        if (modelHit != NULL)
        {
      			screen[j][i] = modelHit->getColor(e, e + t * (s - e), lights, models, depth);
        }
      		else
      			screen[j][i] = png::rgb_pixel(0,0,0);

  		}
  }