#ifndef __CAMERAH__
#define __CAMERAH__

#include <vector>
#include "Vector3D.h"
#include "Model.h"
#include "Light.h"
#include "png++/png.hpp"


using namespace sivelab;
class Camera
{
  protected:
  
  Vector3D pos;
  Vector3D u,v,w; //coords
  float d; //focal length
  float imagePlaneX, imagePlaneY; //height and width of the image plane
  int imagePixelX, imagePixelY; //The number of pixels in the X and Y directions
  int depth;
  
  public:

  	/*
    calculates the color at each pixel of the image plane
  	 */
	virtual void drawScene(png::image < png::basic_rgb_pixel <unsigned char> > & screen, const std::vector<Model *> models, const std::vector<Light *> lights) const = 0;

  /*
	Default Constructor
   */
  Camera()
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
  }

  /*
	Sets a new image size
   */
  void setImageSize(const float width, const float height)
  {
	imagePlaneX = width;
	imagePlaneY = height;
  }

  /*
	Sets the pixels in the image plane
   */
  void setImagePixels(const int width, const int height)
  {
	imagePixelX = width;
	imagePixelY = height;
  }

  /*
   Sets the camera to a new position
  */
  void setLocation(const Vector3D newPos)
  {
	pos = newPos;
  }

  /*
  Sets the camera to a new direction
  */
  void setDirection(const Vector3D newDir)
  {
    w = newDir;
    w.normalize();
    w *= -1;
    Vector3D t(0,1,0);
    u = t.cross(w);
    u.normalize();
    v = u.cross(w);
    v.normalize();
  }
  
  /*
	Returns the location of the camera
   */
  Vector3D getPosition() const
  { return pos; }

  /*
	Returns distance to the image plane
   */
  float getFocalLength() const
  { return d; }

  /*
  sets a new focal length
  */
  float setFocalLength(const float newLen)
  { d = newLen; }

  /*
	returns the number of pixels in the image plane's width
   */
  int getImagePlanePixX() const
  { return imagePixelX; }

  /*
	returns the number of pixels in the image plane's height
   */
  int getImagePlanePixY() const
  { return imagePixelY; }

  /*
   returns the width of the imagePlane
  */
  float getImagePlaneX()
  { return imagePlaneX; }

  /*
   returns the height of the imagePlane
  */
  float getImagePlaneY()
  { return imagePlaneY; }
};

#endif
