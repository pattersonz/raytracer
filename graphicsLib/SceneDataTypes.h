#ifndef __SCENEDATA_H__
#define __SCENEDATA_H__ 1

#include "Matrix4x4.h"
using namespace sivelab;

//
// Scene Parameters
//
struct sceneParamData 
{
    Vector3D backgroundColor;
    bool usesEnvMap;
    std::string envMapPrefix;
};


// 
// Camera Initialization Data
//
enum CamType { 
  perspective, orthographic 
};
struct cameraData {
  CamType type;
  Vector3D position;
  Vector3D gazeDirection;
  float focalLength;
  float imagePlaneWidth;
  float aspectRatio;
};


//
// Light initialization Data
//
enum LightType {
    point, area
};
struct lightData 
{
    LightType type;
    Vector3D position;
    Vector3D intensity;
    Vector3D normal;
    float width, length;
};


//
// Shader initialization Data
//
enum ShaderType {
    lambertian, blinnphong, phong, mirror, glaze, dielectric, blinnPhongMirrored
};
struct shaderData 
{
    ShaderType type;
    std::string name;
    Vector3D kd_diffuse;
    Vector3D ks_specular;
    float phongExp;
    float mirrorCoef;
    float refractiveIndex;
    Vector3D attenuationCoef;
};


//
// Shape initialization Data
//
enum ShapeType {
    sphere, triangle, box, Sinstance, mesh
};
struct shapeData
{
    ShapeType type;
    std::string name;

    float radius;
    Vector3D center;

    Vector3D minPt;
    Vector3D maxPt;

    Vector3D v0, v1, v2;

    shaderData shader;

    Vector3D translate;
    Vector3D scale;
    float xRotate;
    float yRotate;
    float zRotate;

    std::string file;

    Matrix4x4 m;

    std::string id;
};


#endif
