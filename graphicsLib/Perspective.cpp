#include "Perspective.h"
#include <stdio.h>

void Perspective::rayTraceScene(png::image < png::basic_rgb_pixel <unsigned char> > & screen, const std::vector<Model *> models, const std::vector<Light *> lights) const
  {
    std::clock_t start;
    double duration;

    start = std::clock();

    int raysPerPix = rpp;

    srand(time(NULL));
    BVA* bva = BVA::createBVA(models);
    printf("BVH created\n");
  	  	//location of camera
  	Vector3D e = pos;

  	//location of pixel
  	float pixelDimX = imagePlaneX / (float)imagePixelX;
  	float pixelDimY = imagePlaneY / (float)imagePixelY;

    int rppD = (int)(sqrt(raysPerPix));
    raysPerPix = rppD * rppD;


    Vector3D s;
  	//get color at each pixel
  	for (int i = 0; i < imagePixelX; i++)
    {
  		for (int j = 0; j < imagePixelY; j++)
  		{
        png::rgb_pixel* cellColors = new png::rgb_pixel[raysPerPix];

        for (int r = 0; r < raysPerPix; r++)
        {

          float offsetX, offsetY;
          if ( !antiAl )
          {
    			  offsetX = (((float)i + 0.5f) * pixelDimX) - imagePlaneX / 2.0f;
      		  offsetY = (((float)j + 0.5f) * pixelDimY) - imagePlaneY / 2.0f;
          }
          else
          {
            int col = r % rppD;
            int row = r / rppD;
            float cell = 1.0f / rppD;

            float randX =((float) rand() / (float)(RAND_MAX));
            float randY =((float) rand() / (float)(RAND_MAX));
            offsetX = (((float)i + (col * cell + randX) ) * pixelDimX) - imagePlaneX / 2.0f;
            offsetY = (((float)j + (row * cell + randY) ) * pixelDimY) - imagePlaneY / 2.0f;
          }

    			// origin + (d * -w) + offsetX * u + offsetY * v
  		    s[0] = e[0] + d * -1 * w[0] + offsetX * u[0] + offsetY * v[0];
  		    s[1] = e[1] + d * -1 * w[1] + offsetX * u[1] + offsetY * v[1];
  		    s[2] = e[2] + d * -1 * w[2] + offsetX * u[2] + offsetY * v[2];

  		    float t = 0.0f;

  		      	//direction of the ray
  		  	Vector3D dir = s - e;

  		    Model * modelHit = 0;
          Vector3D norm(1,0,0);
  		  	//for all models in the scene, if the array collides a closer space, update the color
  		  	bva->checkCollisions(dir, e, 1.0f, t, modelHit, norm);

  		    //printf("%lf : %d\n", t, modelHit == 0);
          if (modelHit != 0)
          {
        			cellColors[r] = modelHit->getColor(e, e + t * (s - e), t, lights, bva, depth, norm);
          }
        		else
        			cellColors[r] = png::rgb_pixel(0,0,0);
        }

        float red = 0.0f, green = 0.0f, blue = 0.0f;
        for (int r = 0; r < raysPerPix; r++)
        {
          red += (float)cellColors[r].red / (float)raysPerPix;
          green += (float)cellColors[r].green / (float)raysPerPix;
          blue += (float)cellColors[r].blue / (float)raysPerPix;
        }
        screen[j][i] = png::rgb_pixel((int)(red), (int)(green), (int)(blue));

  		}
      std::cout << "Row: " << i << '\r' << std::flush;
    }

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout<<"Render Time: "<< duration <<'\n';
  }

  void Perspective::rasterizeScene(png::image < png::basic_rgb_pixel <unsigned char> > & screen, const std::vector<Model *> models, const std::vector<Light *> lights) const
  {

    float* zBuf = new float[imagePixelX * imagePixelY];
    for ( int i = 0; i < imagePixelX * imagePixelY; i++)
      zBuf[i] = -1.0f;
    glm::mat4 projMat = getMperspective(imagePlaneX / 2.0f, -imagePlaneY / 2.0f, d);
    glm::mat4 viewMatrix = getCamMatrix();
    glm::mat4 Mvp = getViewPortMatrix();


    for (int i = 0; i < models.size(); i++)
    {

      Figure f(models[i]->getID(), models[i]->getListOfTris(), models[i]->getShader());
      glm::mat4 M = viewMatrix * f.getModelMatrix();

      f.rasterizeFigure(screen, pos, lights, M, viewMatrix, projMat, Mvp, zBuf, imagePixelX, imagePixelY);
    }
  }

  glm::mat4 Perspective::getProjectionMatrix(float r, float t, float n) const 
  {
    //update
    float vFov = /*-3.14159/4.0; // */atan2(t, fabs(n)) * 2.0f;
    float aspectRatio = 1.0f;
    std::cout << "vFOV = " << vFov * 180.0f / 3.14159f << " tanVal" << atan2(t, fabs(n))<< std::endl;
    return glm::perspective(vFov, aspectRatio, n, 200.0f);
  }

glm::mat4 Perspective::getMperspective(const float r, const float t, const float n) const
{
  float l = -r, b = -t, f = 1000 ;
  float n2 = 2 * n;

  return glm::transpose( glm::mat4(2/(r-l), 0, 0, -1 * (r+l)/(r-l),
                        0, 2/(t-b), 0, -1 * (t+b)/(t-b),
                        0, 0, 2/(n-f), -1 * (n+f)/(n-f),
                        0, 0, 0, 1)) * 
          glm::transpose( glm::mat4(n, 0, 0, 0,
                                    0, n, 0, 0,
                                    0, 0, n+f, -f * n,
                                    0, 0, 1, 0) );
  /*
  return glm::transpose(glm::mat4( n2/(r - l), 0, (l+r)/(l-r), 0,
		    0, n2/(t-b), (b+t)/(b-t), 0,
		    0, 0, (f+n)/(n-f), n2*f/(f-n),
		    0,0,1,0 ));*/
}
