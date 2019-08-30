#include "Sphere.h"

#define BETWEEN(x,y,z) ( (x) < (y) && (y) < (z) ? true : ((x) > (y) && (y) > (z) ? true : false) )

bool Sphere::getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& modelHit, Vector3D& norm)
{
  float t, t2;
  float A, B, C;
  A = d.dot(d);
  B = d.dot(e - position);
  C = (e-position).dot(e-position) - radius * radius;
  float disc = B * B  -  A * C;
  if (disc < 0)
    return false;

  t= (-1 * B - sqrt(disc)) / (A);

  if (B < 0 && t < minT)
    t = (-1 * B + sqrt(disc)) / (A);
    

  if ( t > minT && (maxT > t || maxT == 0.0f))
  {
  	maxT = t;
    Vector3D newNorm = (e + t * d) - position;
    newNorm.normalize();
    norm = newNorm;
  	return true;
  }
  return false;
  
}

png::rgb_pixel Sphere::getNormalAsColor(Vector3D interP) const
{
		interP -= position;
		interP.normalize();
		return png::rgb_pixel((interP[0] + 1.0f) / 2.0f * 255,
							  (interP[1] + 1.0f) / 2.0f * 255,
							  (interP[2] + 1.0f) / 2.0f * 255 );
}



std::vector<Triangle*> Sphere::getListOfTris() const
{
  std::vector<Triangle*> v;


  //Set first ring of triangles for top and bottom
  glm::vec3 center(position[0], position[1], position[2]);

  ModelOBJ::Vertex top, bot, left, right, front, back;
  top.position[0] = position[0]; top.position[1] = position[1] + radius; top.position[2] = position[2];
  top.normal[0] = 0; top.normal[1] = 1; top.normal[2] = 0;

  bot.position[0] = position[0]; bot.position[1] = position[1] - radius; bot.position[2] = position[2];
  bot.normal[0] = 0; bot.normal[1] = -1; bot.normal[2] = 0;

  left.position[0] = position[0] + radius; left.position[1] = position[1]; left.position[2] = position[2];
  left.normal[0] = 1; left.normal[1] = 0; left.normal[2] = 0;

  right.position[0] = position[0] - radius; right.position[1] = position[1]; right.position[2] = position[2];
  right.normal[0] = -1; right.normal[1] = 0; right.normal[2] = 0;

  front.position[0] = position[0]; front.position[1] = position[1]; front.position[2] = position[2] + radius;
  front.normal[0] = 0; front.normal[1] = 0; front.normal[2] = 1;

  back.position[0] = position[0]; back.position[1] = position[1] - radius; back.position[2] = position[2] - radius;
  back.normal[0] = 0; back.normal[1] = 0; back.normal[2] = -1;

  Triangle* t = new Triangle(id, top, left, front, shade);
  v.push_back(t);
  t = new Triangle(id, top, left, back, shade);
  v.push_back(t);

  t = new Triangle(id, top, right, front, shade);
  v.push_back(t);
  t = new Triangle(id, top, right, back, shade);
  v.push_back(t);

  t = new Triangle(id, bot, left, front, shade);
  v.push_back(t);
  t = new Triangle(id, bot, left, back, shade);
  v.push_back(t);

  t = new Triangle(id, bot, right, front, shade);
  v.push_back(t);
  t = new Triangle(id, bot, right, back, shade);
  v.push_back(t);


  for (int i = 0; i < 2; i++)
  {
    std::vector<Triangle *> temp;
    for (int j = 0; j < v.size(); j++)
    {
      ModelOBJ::Vertex* tV;
      v[j]->getVerticies(tV);

      ModelOBJ::Vertex a,b,c,ab,ac,bc;
      glm::vec3 posA, posB, posC, posT;

      a = tV[0]; posA = glm::vec3(a.position[0], a.position[1], a.position[2]);
      b = tV[1]; posB = glm::vec3(b.position[0], b.position[1], b.position[2]);
      c = tV[2]; posC = glm::vec3(c.position[0], c.position[1], c.position[2]);

      //get ab
      posT = (posA + posB) * 0.5f;
      posT -= center;
      posT = glm::normalize(posT);
      ab.normal[0] = posT.x; ab.normal[1] = posT.y; ab.normal[2] = posT.z;
      posT *= radius;
      posT += center;
      ab.position[0] = posT.x; ab.position[1] = posT.y; ab.position[2] = posT.z;


      //get ac
      posT = (posA + posC) * 0.5f;
      posT -= center;
      posT = glm::normalize(posT);
      ac.normal[0] = posT.x; ac.normal[1] = posT.y; ac.normal[2] = posT.z;
      posT *= radius;
      posT += center;
      ac.position[0] = posT.x; ac.position[1] = posT.y; ac.position[2] = posT.z;

      //get bc
      posT = (posB + posC) * 0.5f;
      posT -= center;
      posT = glm::normalize(posT);
      bc.normal[0] = posT.x; bc.normal[1] = posT.y; bc.normal[2] = posT.z;
      posT *= radius;
      posT += center;
      bc.position[0] = posT.x; bc.position[1] = posT.y; bc.position[2] = posT.z;

      Triangle * t1 = new Triangle(id, a, ab, ac, shade);
      Triangle * t2 = new Triangle(id, ab, b, bc, shade);
      Triangle * t3 = new Triangle(id, ac, bc, c, shade);
      Triangle * t4 = new Triangle(id, ab, bc, ac, shade);
      temp.push_back(t1);
      temp.push_back(t2);
      temp.push_back(t3);
      temp.push_back(t4);
    }

    v.clear();

    for (int j = 0; j < temp.size(); j++)
      v.push_back(temp[j]);
  }

  return v;
}

void Sphere::getBoundaries(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax)
{
  xmin = position[0] - radius;
  xmax = position[0] + radius;
  ymin = position[1] - radius;
  ymax = position[1] + radius;
  zmin = position[2] - radius;
  zmax = position[2] + radius;
}