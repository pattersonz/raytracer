#include "Triangle.h"

#define G d[0]
#define H d[1]
#define I d[2]

/*bool Triangle::getCollisions(const Vector3D d, const Vector3D e, const float minT, float& maxT, Model*& modelHit)
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
  return true;
  
}

png::rgb_pixel Triangle::getNormalAsColor(Vector3D interP) const
{


	return png::rgb_pixel((norm[0] + 1.0f) / 2.0f * 255,
							  (norm[1] + 1.0f) / 2.0f * 255,
							  (norm[2] + 1.0f) / 2.0f * 255 );
}

png::rgb_pixel Triangle::getColor(const Vector3D e, const Vector3D intersect, const std::vector<Light*> lights, const std::vector<Model *> models, int depth) const
{
    if (! shade->getReflective())
  	return shade->getColor(e, intersect, norm, lights, isPathToLight(intersect, lights, models));
  else
  	return getNextColor(e, intersect, norm, lights, models, depth);
}
*/

void Triangle::glInit(Vector3D a, Vector3D aN, Vector3D aC, Vector3D b, Vector3D bN,
  Vector3D bC, Vector3D c, Vector3D cN, Vector3D cC, sivelab::GLSLObject shaderN)
{
  shader = shaderN;
  pMatID = shader.createUniform("projMatrix");
  
  GLuint m_triangleVBO;
  glGenBuffers(1, &m_triangleVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO);

  int numFloatsNeeded = 3 * 9;
  float* host_VertexBuffer = new float[ numFloatsNeeded ];

  int tIdx = 0;

  // V0
  host_VertexBuffer[ tIdx++ ] = a[0]; 
  host_VertexBuffer[ tIdx++ ] = a[1]; 
  host_VertexBuffer[ tIdx++ ] = a[2];

  aN.normalize();

  host_VertexBuffer[ tIdx++ ] = aN[0]; 
  host_VertexBuffer[ tIdx++ ] = aN[1]; 
  host_VertexBuffer[ tIdx++ ] = aN[2];

  host_VertexBuffer[ tIdx++ ] = aC[0]; 
  host_VertexBuffer[ tIdx++ ] = aC[1]; 
  host_VertexBuffer[ tIdx++ ] = aC[2];

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

  int numBytes = (numFloatsNeeded) * sizeof(float);
  glBufferData(GL_ARRAY_BUFFER, numBytes, host_VertexBuffer, GL_STATIC_DRAW);
  delete [] host_VertexBuffer;


  //create VAO

  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (const GLvoid *)12);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (const GLvoid *)24);

  glBindVertexArray(0);
}

void Triangle::glRender(glm::mat4 projMatrix)
{

    shader.activate();

    glUniformMatrix4fv(pMatID, 1, GL_FALSE, glm::value_ptr(projMatrix));

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);  

    glBindVertexArray(0);

    shader.deactivate();

}
