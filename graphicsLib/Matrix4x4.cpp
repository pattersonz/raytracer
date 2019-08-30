// MIT License
//
// Copyright (c) 2012,2017,2018 Pete Willemsen
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Matrix4x4.h"

using namespace sivelab;

Matrix4x4::Matrix4x4(const double a11, const double a12, const double a13, const double a14,
		     const double a21, const double a22, const double a23, const double a24,
		     const double a31, const double a32, const double a33, const double a34,
		     const double a41, const double a42, const double a43, const double a44)
{
  d[0][0]=a11;  d[0][1]=a12;  d[0][2]=a13; d[0][3] = a14;
  d[1][0]=a21;  d[1][1]=a22;  d[1][2]=a23; d[1][3] = a24;
  d[2][0]=a31;  d[2][1]=a32;  d[2][2]=a33; d[2][3] = a34;
  d[3][0]=a41;  d[3][1]=a42;  d[3][2]=a43; d[3][3] = a44;
}

void Matrix4x4::set(const double a11, const double a12, const double a13, const double a14,
		    const double a21, const double a22, const double a23, const double a24,
		    const double a31, const double a32, const double a33, const double a34,
		    const double a41, const double a42, const double a43, const double a44)
{
  d[0][0]=a11;  d[0][1]=a12;  d[0][2]=a13; d[0][3] = a14;
  d[1][0]=a21;  d[1][1]=a22;  d[1][2]=a23; d[1][3] = a24;
  d[2][0]=a31;  d[2][1]=a32;  d[2][2]=a33; d[2][3] = a34;
  d[3][0]=a41;  d[3][1]=a42;  d[3][2]=a43; d[3][3] = a44;
}

void Matrix4x4::print()
{
  std::cout << d[0][0] << ' ' << d[0][1] << ' ' << d[0][2] << ' ' << d[0][3] << std::endl;
  std::cout << d[1][0] << ' ' << d[1][1] << ' ' << d[1][2] << ' ' << d[1][3] << std::endl;
  std::cout << d[2][0] << ' ' << d[2][1] << ' ' << d[2][2] << ' ' << d[2][3] << std::endl;
  std::cout << d[3][0] << ' ' << d[3][1] << ' ' << d[3][2] << ' ' << d[3][3] << std::endl;
}


void Matrix4x4::setIdentity(void)
{
  for (unsigned int i=0; i<4; i++)
    for (unsigned int j=0; j<4; j++)
      d[i][j] = 0.0;
  d[0][0] = d[1][1] = d[2][2] = d[3][3] = 1.0;
}

const Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &b) const
{
  Matrix4x4 m;
  
      m.d[0][0] = d[0][0]*b.d[0][0] + d[0][1]*b.d[1][0] + d[0][2]*b.d[2][0] + d[0][3]*b.d[3][0];
      m.d[0][1] = d[0][0]*b.d[0][1] + d[0][1]*b.d[1][1] + d[0][2]*b.d[2][1] + d[0][3]*b.d[3][1];
      m.d[0][2] = d[0][0]*b.d[0][2] + d[0][1]*b.d[1][2] + d[0][2]*b.d[2][2] + d[0][3]*b.d[3][2];
      m.d[0][3] = d[0][0]*b.d[0][3] + d[0][1]*b.d[1][3] + d[0][2]*b.d[2][3] + d[0][3]*b.d[3][3];

      m.d[1][0] = d[1][0]*b.d[0][0] + d[1][1]*b.d[1][0] + d[1][2]*b.d[2][0] + d[1][3]*b.d[3][0];
      m.d[1][1] = d[1][0]*b.d[0][1] + d[1][1]*b.d[1][1] + d[1][2]*b.d[2][1] + d[1][3]*b.d[3][1];
      m.d[1][2] = d[1][0]*b.d[0][2] + d[1][1]*b.d[1][2] + d[1][2]*b.d[2][2] + d[1][3]*b.d[3][2];
      m.d[1][3] = d[1][0]*b.d[0][3] + d[1][1]*b.d[1][3] + d[1][2]*b.d[2][3] + d[1][3]*b.d[3][3];

      m.d[2][0] = d[2][0]*b.d[0][0] + d[2][1]*b.d[1][0] + d[2][2]*b.d[2][0] + d[2][3]*b.d[3][0];
      m.d[2][1] = d[2][0]*b.d[0][1] + d[2][1]*b.d[1][1] + d[2][2]*b.d[2][1] + d[2][3]*b.d[3][1];
      m.d[2][2] = d[2][0]*b.d[0][2] + d[2][1]*b.d[1][2] + d[2][2]*b.d[2][2] + d[2][3]*b.d[3][2];
      m.d[2][3] = d[2][0]*b.d[0][3] + d[2][1]*b.d[1][3] + d[2][2]*b.d[2][3] + d[2][3]*b.d[3][3];

      m.d[3][0] = d[3][0]*b.d[0][0] + d[3][1]*b.d[1][0] + d[3][2]*b.d[2][0] + d[3][3]*b.d[3][0];
      m.d[3][1] = d[3][0]*b.d[0][1] + d[3][1]*b.d[1][1] + d[3][2]*b.d[2][1] + d[3][3]*b.d[3][1];
      m.d[3][2] = d[3][0]*b.d[0][2] + d[3][1]*b.d[1][2] + d[3][2]*b.d[2][2] + d[3][3]*b.d[3][2];
      m.d[3][3] = d[3][0]*b.d[0][3] + d[3][1]*b.d[1][3] + d[3][2]*b.d[2][3] + d[3][3]*b.d[3][3];

  return m;
}

//const Vector3D Matrix4x4::multVector(const Vector3D &v, const double w) const
//{
//  Vector3D result(0,0,0);
//  result[0] = d[0][0] * v[0] + d[0][1] * v[1] + d[0][2] * v[2] + d[0][3] * w;
//  result[1] = d[1][0] * v[0] + d[1][1] * v[1] + d[1][2] * v[2] + d[1][3] * w;
//  result[2] = d[2][0] * v[0] + d[2][1] * v[1] + d[2][2] * v[2] + d[2][3] * w;
//  return result;
//}

const Vector3D Matrix4x4::multVector(const Vector3D &v, double &w) const
{
  Vector3D result(0,0,0);

  result[0] = d[0][0] * v[0] + d[0][1] * v[1] + d[0][2] * v[2] + d[0][3] * w;
  result[1] = d[1][0] * v[0] + d[1][1] * v[1] + d[1][2] * v[2] + d[1][3] * w;
  result[2] = d[2][0] * v[0] + d[2][1] * v[1] + d[2][2] * v[2] + d[2][3] * w;

  w         = d[3][0] * v[0] + d[3][1] * v[1] + d[3][2] * v[2] + d[3][3] * w;
  
  return result;
}

double Matrix4x4::det(void) const
{
  //
  // cofactor analysis to compute determinant using the first row
  //

  double a = 0.0;
  double det = 0.0;
  for (int i=0; i<4; i++)
    {
      a = cofactor(0,i);
      det = det + d[0][i]*a;
    }
  return det;  
}

double Matrix4x4::cofactor(int i, int j) const
{
  Matrix3x3 tmp;
  for (int x=0; x<4; x++)
    for (int y=0; y<4; y++)
      {
	if ((x != i) && (y != j)) 
	  {
	    if ((x > i) && (y > j))
	      tmp.d[x-1][y-1] = d[x][y];
	    else if (x > i)
	      tmp.d[x-1][y] = d[x][y];
	    else if (y > j)
	      tmp.d[x][y-1] = d[x][y];
	    else 
	      tmp.d[x][y] = d[x][y];
	  }
      }
  
  double cofactor_value;
  if (((i+1) + (j+1)) % 2 == 0)  // even
    cofactor_value = tmp.det();
  else
    cofactor_value = -1.0 * tmp.det();
  return cofactor_value;
}

const Matrix4x4 Matrix4x4::inverse(void) const
{
  Matrix4x4 adjoint;
  adjoint.set( cofactor(0,0), cofactor(1,0), cofactor(2,0), cofactor(3,0),
	       cofactor(0,1), cofactor(1,1), cofactor(2,1), cofactor(3,1),
	       cofactor(0,2), cofactor(1,2), cofactor(2,2), cofactor(3,2),
	       cofactor(0,3), cofactor(1,3), cofactor(2,3), cofactor(3,3) );

  double inv_det = 1.0 / det();
  return adjoint * inv_det;
}

const Matrix4x4 Matrix4x4::transpose(void) const
{
  Matrix4x4 t( d[0][0], d[1][0], d[2][0], d[3][0],
	       d[0][1], d[1][1], d[2][1], d[3][1],
	       d[0][2], d[1][2], d[2][2], d[3][2], 
	       d[0][3], d[1][3], d[2][3], d[3][3] );
  return t;
}

void Matrix4x4::makeTranslation(const double x, const double y, const double z)
{
  setIdentity();
  d[0][3] = x;
  d[1][3] = y;
  d[2][3] = z;
}

void Matrix4x4::makeScale(const double x, const double y, const double z)
{
  setIdentity();
  d[0][0] = x;
  d[1][1] = y;
  d[2][2] = z;
}

void Matrix4x4::makeRotationX( const double rad )
{
  d[0][0] = 1;  d[0][1] = 0;          d[0][2] = 0;          d[0][3] = 0;
  d[1][0] = 0;  d[1][1] = cos(rad);   d[1][2] = -sin(rad);  d[1][3] = 0;
  d[2][0] = 0;  d[2][1] = sin(rad);   d[2][2] = cos(rad);   d[2][3] = 0;
  d[3][0] = 0;  d[3][1] = 0;          d[3][2] = 0;          d[3][3] = 1;
}

void Matrix4x4::makeRotationY( const double rad )
{
  d[0][0] = cos(rad);   d[0][1] = 0;  d[0][2] = sin(rad);  d[0][3] = 0;
  d[1][0] = 0;          d[1][1] = 1;  d[1][2] = 0;         d[1][3] = 0;
  d[2][0] = -sin(rad);  d[2][1] = 0;  d[2][2] = cos(rad);  d[2][3] = 0;
  d[3][0] = 0;          d[3][1] = 0;  d[3][2] = 0;         d[3][3] = 1;
}

void Matrix4x4::makeRotationZ( const double rad )
{
  d[0][0] = cos(rad);  d[0][1] = -sin(rad);  d[0][2] = 0;  d[0][3] = 0;
  d[1][0] = sin(rad);  d[1][1] = cos(rad);   d[1][2] = 0;  d[1][3] = 0;
  d[2][0] = 0;         d[2][1] = 0;          d[2][2] = 1;  d[2][3] = 0;
  d[3][0] = 0;         d[3][1] = 0;          d[3][2] = 0;  d[3][3] = 1;
}


double Matrix3x3::det(void) const
{
  return d[0][0]*d[1][1]*d[2][2] - d[0][0]*d[2][1]*d[1][2] - 
    d[1][0]*d[0][1]*d[2][2] + d[1][0]*d[2][1]*d[0][2] + 
    d[2][0]*d[0][1]*d[1][2] - d[2][0]*d[1][1]*d[0][2];
}


