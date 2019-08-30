#ifndef __MATRIX4x4_H__
#define __MATRIX4x4_H__ 1

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

#include "Vector3D.h"

class Matrix3x3
{
public:
  double d[3][3];
  double det(void) const;
};

class Matrix4x4 
{
public:
  double d[4][4];

  Matrix4x4() {};
  Matrix4x4(const double a11, const double a12, const double a13, const double a14,
	    const double a21, const double a22, const double a23, const double a24,
	    const double a31, const double a32, const double a33, const double a34,
	    const double a41, const double a42, const double a43, const double a44);

  void set(const double, const double, const double, const double, 
	   const double, const double, const double, const double, 
	   const double, const double, const double, const double, 
	   const double, const double, const double, const double);

  void print();

  void setIdentity(void);

  const Matrix4x4 operator*(const Matrix4x4 &) const;

  // to make implementations simple, you have to supply the w vector
  // here, rather than creating a Vector4D.  So, you will need to
  // create a variable to hold the w coordinate and pass it along as
  // it changes...
  //
  // double w = 1.0;
  // Matrix4x4 M;
  // Vector3D p;    
  // ...
  // M.multVector( p, w );
  //     
  // w will then contain the transformed w that you will need
  // eventually for the perspective divide
  //     
  const sivelab::Vector3D multVector(const sivelab::Vector3D &v, double &w) const;

  double det(void) const; 
  double cofactor(int i, int j) const;
  const Matrix4x4 inverse(void) const;
  const Matrix4x4 transpose(void) const;       // return the transpose of self

  void makeTranslation(const double x, const double y, const double z);
  void makeScale(const double x, const double y, const double z);

  void makeRotationX( const double angle_in_radians );
  void makeRotationY( const double angle_in_radians );
  void makeRotationZ( const double angle_in_radians );
};

inline const Matrix4x4 operator*(const Matrix4x4& m, const double c)
{
  return Matrix4x4(m.d[0][0]*c, m.d[0][1]*c, m.d[0][2]*c, m.d[0][3]*c,
		   m.d[1][0]*c, m.d[1][1]*c, m.d[1][2]*c, m.d[1][3]*c,
		   m.d[2][0]*c, m.d[2][1]*c, m.d[2][2]*c, m.d[2][3]*c,
		   m.d[3][0]*c, m.d[3][1]*c, m.d[3][2]*c, m.d[3][3]*c);
}

#endif

