#ifndef __CAMERAH__
#define __CAMERAH__

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cmath>
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
  bool antiAl;
  int rpp;
  public:

  	/*
    calculates the color at each pixel by sending rays from the camera and returning the color of objects hit
  	 */
	virtual void rayTraceScene(png::image < png::basic_rgb_pixel <unsigned char> > & screen, const std::vector<Model *> models, const std::vector<Light *> lights) const = 0;

    /*
    calculates the color at each pixel by transforming shapes onto the screen and updating pixels
    */
  virtual void rasterizeScene(png::image < png::basic_rgb_pixel <unsigned char> > & screen, const std::vector<Model *> models, const std::vector<Light *> lights) const = 0;
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
  antiAl = false;
  rpp = 1;
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
  Sets the rays per pixel in the camera, if value is not 1 turns on
  anti alising
  */
  void setRPP(int rays)
  {
    if (rays > 1)
    {
      rpp = rays;
      antiAl = true;
    }
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

  void rotateGaze(const double xShift, const double yShift)
  {
    double cosThetaX = cos(xShift/100);
    double sinThetaX = sin(xShift/100);
    double cosThetaY = cos(yShift/100);
    double sinThetaY = sin(yShift/100);

    glm::mat3 basis = glm::transpose(glm::mat3(u[0], v[0], -1 * w[0], u[1], v[1], -1 * w[1], u[2], v[2],  -1 * w[2]));
 
    //access align the current
    Vector3D uOnXY = Vector3D(u[0], u[1], 0);
    uOnXY.normalize();
    float aUXY = acos(Vector3D(1,0,0).dot(uOnXY)); //rotate about Y

    Vector3D vOnYZ = Vector3D(0, v[1], v[2]);
    vOnYZ.normalize();
    float aVYZ = acos(Vector3D(0,1,0).dot(vOnYZ)); //rotate about Z

    glm::mat3 toXY = glm::transpose(glm::mat3( cos(aUXY), 0, sin(aUXY),
                      0,        1,        0,
                  -1 * sin(aUXY), 0, cos(aUXY)));

    glm::mat3 toYZ = glm::transpose(glm::mat3(1, 0, 0,
                      0, cos(aVYZ), -1 * sin(aVYZ),
                      0, sin(aVYZ), cos(aVYZ)));

    //make new rotations
    glm::mat3 yRot = glm::transpose(glm::mat3(cosThetaX, 0, sinThetaX,
                          0, 1, 0,
                      -1 * sinThetaX, 0, cosThetaX));

    glm::mat3 xRot = glm::transpose(glm::mat3(1, 0, 0,
                      0, cosThetaY, -1 * sinThetaY,
                      0, sinThetaY, cosThetaY));

    glm::mat3 rotations = xRot * yRot;
    
    //align to axis, then apply roation by given values, then inverse alignment to axis

    basis = glm::inverse(toYZ) * glm::inverse(toXY) * rotations * toXY * toYZ * basis;

    w = -1* Vector3D(basis[2][0], basis[2][1], basis[2][2]);
    v = Vector3D(basis[1][0], basis[1][1], basis[1][2]);
    u = Vector3D(basis[0][0], basis[0][1], basis[0][2]);
  }
  
  /*
	Returns the location of the camera
   */
  Vector3D getPosition() const
  { return pos; }

  /*
  Adds the following values to the position, in relation to the camera
  */
  void updatePos(Vector3D change)
  { 
    Vector3D uUpdate = change[0] * u;
    Vector3D vUpdate = change[1] * v;
    Vector3D wUpdate = change[2] * w;
    pos += uUpdate + vUpdate + wUpdate;
     }
  
  /*
	Returns distance to the image plane
   */
  float getFocalLength() const
  { return d; }

  /*
  sets a new focal length
  */
  void setFocalLength(const float newLen)
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

  /*
  returns the projectionMatrix, either orthographic
  or perspective depending on the camera type.
  */
  virtual glm::mat4 getProjectionMatrix(float r, float t, float n) const = 0;

  /*
  returns the viewMatrix
  */
  glm::mat4 getViewMatrix() const 
  {     return glm::lookAt( glm::vec3(pos[0], pos[1], pos[2]), glm::vec3(pos[0], pos[1], pos[2])
            - glm::vec3(w[0], w[1], w[2]), glm::vec3(v[0], v[1], v[2])); 
  }

  /*
  returns the camera Matrix
  */
  glm::mat4 getCamMatrix() const
  {     return glm::transpose(glm::mat4(u[0], u[1], u[2], 0,
                              v[0], v[1], v[2], 0,
                              w[0], w[1], w[2], 0,
                              0,0,0,1)) *
               glm::transpose(glm::mat4(1,0,0,-pos[0],
                                        0,1,0,-pos[1],
                                        0,0,1,-pos[2],
                                        0,0,0,1) );
  }

  /*
  calculates and returns the viewport matrix
  */
  glm::mat4 getViewPortMatrix() const 
  {
    return glm::transpose(glm::mat4(imagePixelX / 2, 0, 0, (imagePixelX - 1)/2,
                    0, imagePixelY/2, 0, (imagePixelY  - 1)/2,
                    0, 0, 1, 0,
                    0, 0, 0, 1));
  }
};

#endif
