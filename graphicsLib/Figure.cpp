#include "Figure.h"

#ifndef GLM_SWIZZLE
#define GLM_SWIZZLE
#endif



bool Figure::getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& hitModel, Vector3D& norm)
{
  
  return bva->checkCollisions(d, e, minT, maxT, hitModel, norm);
  
}


png::rgb_pixel Figure::getNormalAsColor(Vector3D interP) const
{
	return png::rgb_pixel(0,0,0);
}


void Figure::glInit(std::vector<Triangle *> trisN, glm::mat4 modelMatrixN, sivelab::GLSLObject shaderN)
{
  tris = trisN;
  modelMatrix = modelMatrixN;
  shader = shaderN;
}
void Figure::glRender(glm::mat4 projMatrix, glm::mat4 viewMatrix)
{
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(viewMatrix * modelMatrix));

    GLint projMatrixID = shader.createUniform( "projMatrix" );
    GLint viewMatrixID = shader.createUniform( "viewMatrix" ); 
    GLint modelMatrixID = shader.createUniform( "modelMatrix" );
    GLint normalMatrixID = shader.createUniform( "normalMatrix" );

    
    shader.activate();

    glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr(projMatrix));
    glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(normalMatrix));


    for(int i = 0; i < tris.size(); i++)
      tris[i]->glRender(shader);


    shader.deactivate();

}

std::vector<Triangle*> Figure::getListOfTris() const
{
  return tris;
}

glm::mat4 Figure::getModelMatrix()
{
  return modelMatrix;
}

#include <stdio.h>
#include <iostream>
void Figure::rasterizeFigure(png::image < png::basic_rgb_pixel <unsigned char> > & screen, const Vector3D camPos, const std::vector<Light *> lights, const glm::mat4 M, const glm::mat4 Mview, const glm::mat4 Mproj, const glm::mat4 Mvp, float* zBuf, const int imagePixelX, const int  imagePixelY) const
{
  for (int i = 0; i < tris.size(); i++)
  {
    Vector3D tN = tris[i]->getNormal();
    glm::vec3 normal(tN[0], tN[1], tN[2]);

    ModelOBJ::Vertex* tV;
    tris[i]->getVerticies(tV);

    glm::mat4 normalMat = glm::transpose(glm::inverse(M));

    glm::vec3 aP(tV[0].position[0], tV[0].position[1], tV[0].position[2]);
    glm::vec3 bP(tV[1].position[0], tV[1].position[1], tV[1].position[2]);
    glm::vec3 cP(tV[2].position[0], tV[2].position[1], tV[2].position[2]);
    
    glm::vec3* dirToLights = new glm::vec3[lights.size() * 3];
    glm::vec3* halfToLights = new glm::vec3[lights.size() * 3];

    glm::vec4 aM = M * glm::vec4(aP, 1);
    glm::vec4 bM = M * glm::vec4(bP, 1);
    glm::vec4 cM = M * glm::vec4(cP, 1);


    for(int j = 0; j < lights.size(); j++)
    {
      Vector3D lP3D = lights[j]->getPos();
      glm::vec4 lP(lP3D[0], lP3D[1], lP3D[2], 1.0);
      lP = Mview * lP;
      dirToLights[0 + 3 * j] = glm::normalize(glm::vec3(lP.x - aM.x, lP.y - aM.y, lP.z - aM.z));
      dirToLights[1 + 3 * j] = glm::normalize(glm::vec3(lP.x - bM.x, lP.y - bM.y, lP.z - bM.z));
      dirToLights[2 + 3 * j] = glm::normalize(glm::vec3(lP.x - cM.x, lP.y - cM.y, lP.z - cM.z));
      halfToLights[0 + 3 * j] = glm::normalize( glm::normalize( -glm::vec3(aM)) + dirToLights[0 + 3 * j]);
      halfToLights[1 + 3 * j] = glm::normalize( glm::normalize( -glm::vec3(bM)) + dirToLights[1 + 3 * j]);
      halfToLights[2 + 3 * j] = glm::normalize( glm::normalize( -glm::vec3(cM)) + dirToLights[2 + 3 * j]);
    }


    glm::vec4 a = Mvp * Mproj * aM; a.x /= a.w; a.y /= a.w; a.z /= a.w;
    glm::vec4 b = Mvp * Mproj * bM; b.x /= b.w; b.y /= b.w; b.z /= b.w;
    glm::vec4 c = Mvp * Mproj * cM; c.x /= c.w; c.y /= c.w; c.z /= c.w;

    glm::vec4 aN = normalMat * glm::vec4(tV[0].normal[0], tV[0].normal[1], tV[0].normal[2], 0);
    glm::vec4 bN = normalMat * glm::vec4(tV[1].normal[0], tV[1].normal[1], tV[1].normal[2], 0);
    glm::vec4 cN = normalMat * glm::vec4(tV[2].normal[0], tV[2].normal[1], tV[2].normal[2], 0);

    int left, right, top, bot;

    left = getFloorIn(a.x, b.x, c.x, 0);
    right = getRoofIn(a.x + 1, b.x + 1, c.x + 1, imagePixelX - 1);
    top = getFloorIn(a.y, b.y, c.y, 0);
    bot = getRoofIn(a.y + 1, b.y + 1, c.y + 1, imagePixelY - 1);

    for (int x = left; x < right; x++)
      for (int y = top; y < bot; y++)
      {
        float alpha, beta, gamma;

        gamma =  ((a.y - b.y)* x + (b.x - a.x)*y + a.x*b.y - b.x*a.y) / ((a.y - b.y)* c.x + (b.x - a.x)*c.y + a.x*b.y - b.x*a.y);
        beta = ((a.y - c.y)* x + (c.x - a.x)*y + a.x*c.y - c.x*a.y) / ((a.y - c.y)* b.x + (c.x - a.x)*b.y + a.x*c.y - c.x*a.y);

        alpha = 1.0001 - beta - gamma;

        if (alpha >= 0 && beta >= 0 && gamma >= 0)
        {
          float z = alpha * a.z + beta * b.z + gamma * c.z;

          if ( (zBuf[x * imagePixelY + y] == -1) || (z < zBuf[x * imagePixelY + y]))
          {
            zBuf[x * imagePixelY + y] = z;


            glm::vec4 normal = alpha * aN + beta * bN + gamma * cN;

            std::vector<glm::vec3> lightDirs;
            std::vector<glm::vec3> halves;

            for (int u = 0; u < lights.size(); u++)
            {
              glm::vec3 lightD, halfD;
              lightD = alpha * dirToLights[0 + 3 * u] + beta * dirToLights[1 + 3 * u] + gamma * dirToLights[2 + 3 * u];
              halfD = alpha * halfToLights[0 + 3 * u] + beta * halfToLights[1 + 3 * u] + gamma * halfToLights[2 + 3 * u];
              lightDirs.push_back(lightD);
              halves.push_back(halfD);
            }
            screen[y][imagePixelX - 1 - x] = shade->getFragmentColor(normal, lightDirs, halves, lights);

          }
        }


      }

  }
}


int Figure::getFloorIn(float a, float b, float c, int d) const
{
  int cur;
  cur = a < b ? a : b;
  cur = cur < c ? cur : c;

  return cur > d ? cur : d;
}

int Figure::getRoofIn(float a, float b, float c, int d) const
{
  int cur;
  cur = a > b ? a : b;
  cur = cur > c ? cur : c;

  return cur < d ? cur : d;
}

void Figure::getBoundaries(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax)
{
  xmin = bva->xmin;
  xmax = bva->xmax;
  ymin = bva->ymin;
  ymax = bva->ymax;
  zmin = bva->zmin;
  zmax = bva->zmax;
 }

void Figure::updateShader(Shader * newS)
{
  shade = newS;
  bva->updateLeafShaders(newS);
}