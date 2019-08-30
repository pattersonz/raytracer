#include "Triangle.h"

#define G d[0]
#define H d[1]
#define I d[2]
#define LOWEST_OF_THREE(x,y,z) ( (x) <= (y) && (x) <= (z) ? (x) : ( (y) <= (x) && (y) <= (z) ? (y) : (z) ) )
#define HIGHEST_OF_THREE(x,y,z) ( (x) >= (y) && (x) >= (z) ? (x) : ( (y) >= (x) && (y) >= (z) ? (y) : (z) ) )

bool Triangle::getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& modelHit, Vector3D& norm)
{
  float t, beta, gamma, M;
  float A,B,C,D,E,F,J,K,L;

  A = a[0] - b[0];   D = a[0] - c[0];   J = a[0] - e[0];
  B = a[1] - b[1];   E = a[1] - c[1];   K = a[1] - e[1];
  C = a[2] - b[2];   F = a[2] - c[2];   L = a[2] - e[2];
  
  float EIHF = (E * I - H * F);
  float GFDI = (G * F - D * I);
  float DHEG = (D * H - E * G);
  M = A * EIHF + B * GFDI + C * DHEG;

  float AKJB = (A * K - J * B);
  float JCAL = (J * C - A * L);
  float BLKC = (B * L - K * C);

  t = -1 * ( F * AKJB + E * JCAL + D * BLKC) / M;

  if ( ! (t > minT && (maxT > t || maxT == 0.0f)))
    return false;

  gamma = ( I * AKJB + H * JCAL + G * BLKC) / M;

  if (gamma < 0 || gamma > 1)
    return false;

  beta = (J * EIHF + K * GFDI + L * DHEG) / M;

  if ( beta < 0  || beta > 1 - gamma)
    return false;

  maxT = t;
  float alpha(1 - (beta - gamma));
  for (int i = 0; i < 3; i++)
  {
    norm[i] = aV.normal[i] * alpha + bV.normal[i] * beta + cV.normal[i] * gamma; 
  }
  return true;
  
}

png::rgb_pixel Triangle::getNormalAsColor(Vector3D interP) const
{


	return png::rgb_pixel((norm[0] + 1.0f) / 2.0f * 255,
							  (norm[1] + 1.0f) / 2.0f * 255,
							  (norm[2] + 1.0f) / 2.0f * 255 );
}


void Triangle::glInit(Vector3D a, Vector3D aN, Vector3D aC, glm::vec2 aCoord, Vector3D b, Vector3D bN,
      Vector3D bC, glm::vec2 bCoord, Vector3D c, Vector3D cN, Vector3D cC, glm::vec2 cCoord, const GLuint textureID)
{
  
  GLuint m_triangleVBO;
  glGenBuffers(1, &m_triangleVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO);

  //floats for 
  int numFloatsNeeded = 3 * 11;
  float* host_VertexBuffer = new float[ numFloatsNeeded ];

  texID = textureID;

  int tIdx = 0;

  // V0

  //position
  host_VertexBuffer[ tIdx++ ] = a[0]; 
  host_VertexBuffer[ tIdx++ ] = a[1]; 
  host_VertexBuffer[ tIdx++ ] = a[2];

  aN.normalize();

  //normal
  host_VertexBuffer[ tIdx++ ] = aN[0]; 
  host_VertexBuffer[ tIdx++ ] = aN[1]; 
  host_VertexBuffer[ tIdx++ ] = aN[2];


  //color
  host_VertexBuffer[ tIdx++ ] = aC[0]; 
  host_VertexBuffer[ tIdx++ ] = aC[1]; 
  host_VertexBuffer[ tIdx++ ] = aC[2];

  //texture coords
  host_VertexBuffer[ tIdx++ ] = aCoord.x;
  host_VertexBuffer[ tIdx++ ] = aCoord.y;

  // V1
  host_VertexBuffer[ tIdx++ ] = b[0]; 
  host_VertexBuffer[ tIdx++ ] = b[1]; 
  host_VertexBuffer[ tIdx++ ] = b[2];

  bN.normalize();

  host_VertexBuffer[ tIdx++ ] = bN[0]; 
  host_VertexBuffer[ tIdx++ ] = bN[1]; 
  host_VertexBuffer[ tIdx++ ] = bN[2];

  host_VertexBuffer[ tIdx++ ] = bC[0]; 
  host_VertexBuffer[ tIdx++ ] = bC[1]; 
  host_VertexBuffer[ tIdx++ ] = bC[2];

    //texture coords
  host_VertexBuffer[ tIdx++ ] = bCoord.x;
  host_VertexBuffer[ tIdx++ ] = bCoord.y;

  // V2
  host_VertexBuffer[ tIdx++ ] = c[0]; 
  host_VertexBuffer[ tIdx++ ] = c[1]; 
  host_VertexBuffer[ tIdx++ ] = c[2];

  cN.normalize();

  host_VertexBuffer[ tIdx++ ] = cN[0]; 
  host_VertexBuffer[ tIdx++ ] = cN[1]; 
  host_VertexBuffer[ tIdx++ ] = cN[2];

  host_VertexBuffer[ tIdx++ ] = cC[0]; 
  host_VertexBuffer[ tIdx++ ] = cC[1]; 
  host_VertexBuffer[ tIdx++ ] = cC[2];

    //texture coords
  host_VertexBuffer[ tIdx++ ] = cCoord.x;
  host_VertexBuffer[ tIdx++ ] = cCoord.y;

  int numBytes = (numFloatsNeeded) * sizeof(float);
  glBufferData(GL_ARRAY_BUFFER, numBytes, host_VertexBuffer, GL_STATIC_DRAW);
  delete [] host_VertexBuffer;


  //create VAO

  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (const GLvoid *)12);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (const GLvoid *)24);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (const GLvoid *)36);

  glBindVertexArray(0);
}


void Triangle::glRender(sivelab::GLSLObject shader)
{
    GLint  texUnitID = shader.createUniform( "texUnit" );
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glUniform1i(texUnitID, 0);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);  

    glBindVertexArray(0);
}

std::vector<Triangle*> Triangle::getListOfTris() const
{
  std::vector<Triangle*> v;
  Triangle* newT = new Triangle(id, a,b,c,shade);
  v.push_back(newT);
  return v;
}

  Vector3D Triangle::getNormal()
  {
    return norm;
  }


  void Triangle::getVerticies(Vector3D*& tV)
  {
    tV = new Vector3D[3];
    tV[0] = a;
    tV[1] = b;
    tV[2] = c;
  }

  void Triangle::getVerticies(ModelOBJ::Vertex*& tV)
  {
    tV = new ModelOBJ::Vertex[3];
    tV[0] = aV;
    tV[1] = bV;
    tV[2] = cV;
  }

  void Triangle::getBoundaries(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax)
  {
    xmin = LOWEST_OF_THREE(a[0], b[0], c[0]);
    xmax = HIGHEST_OF_THREE(a[0], b[0], c[0]);
    ymin = LOWEST_OF_THREE(a[1], b[1], c[1]);
    ymax = HIGHEST_OF_THREE(a[1], b[1], c[1]);
    zmin = LOWEST_OF_THREE(a[2], b[2], c[2]);
    zmax = HIGHEST_OF_THREE(a[2], b[2], c[2]);
  }