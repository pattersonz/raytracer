// $Id$
/** 
 * @file Scene_instance.cpp File to contain the scene instancing
 * operations.  It is removed from the main Scene.cpp file to keep the
 * contents of the derived SceneElementCreator separate.
 */

// $Log$

#include "SceneFile_Instancer.h"

using namespace sivelab;
using namespace std;

void SceneFile_Instancer::instance( ptree::value_type const &v )
{
  std::istringstream buf;

  //
  // Scene Parameters
  //
  if (v.first == "sceneParameters") {

      sceneParamData spData;
      
    // LookAtPoint and ViewDir are optional, but one should be specified.
    boost::optional<std::string> pBGColor = v.second.get_optional<std::string>("bgColor");
    boost::optional<std::string > pEnvMapPrefix = v.second.get_optional<std::string>("envMapPrefix");

    if (pBGColor) {
      buf.str( *pBGColor );
      buf >> spData.backgroundColor;
      buf.clear();
    } 

    if (pEnvMapPrefix) {
      buf.str( *pEnvMapPrefix );
      buf >> spData.envMapPrefix;
      buf.clear();
      
      spData.usesEnvMap = true;
    }
    else {
      spData.usesEnvMap = false;
    }
  }

  //
  // CAMERA
  //
  if (v.first == "camera") {

    std::string name, type;
    Vector3D position, viewDir, lookatPoint;
    bool lookatSet = false;
    float focalLength;
    float imagePlaneWidth;

    name = v.second.get<std::string>("<xmlattr>.name");
    type = v.second.get<std::string>("<xmlattr>.type");

    buf.str( v.second.get<std::string>("position") );
    buf >> position;
    buf.clear();

    // LookAtPoint and ViewDir are optional, but one should be specified.
    boost::optional<std::string> plookatPoint = v.second.get_optional<std::string>("lookatPoint");
    boost::optional<std::string > pviewDir = v.second.get_optional<std::string>("viewDir");

    if (plookatPoint) {
      lookatSet = true;
      buf.str( *plookatPoint );
      buf >> lookatPoint; 
      buf.clear();
    } else if (pviewDir) {
      buf.str( *pviewDir );
      buf >> viewDir; 
      buf.clear();
    }

    buf.str( v.second.get<std::string>("focalLength") );
    buf >> focalLength;
    buf.clear();

    buf.str( v.second.get<std::string>("imagePlaneWidth") );
    buf >> imagePlaneWidth;
    buf.clear();

    if (lookatSet) {
      viewDir = lookatPoint - position;
      viewDir.normalize();
    }

    struct cameraData cam;
    cam.position = position;
    cam.gazeDirection = viewDir;
    cam.focalLength = focalLength;
    cam.imagePlaneWidth = imagePlaneWidth;
        
    if (type == "perspective")
        cam.type = perspective;
    else if (type == "orthographic")
        cam.type = orthographic;

    // Instance the camera here!
    scene->setCamera(cam);
    std::cout << "Found Camera!" << std::endl;
  }


  //
  // Light
  //
  if (v.first == "light") {

    std::string type;
    sivelab::Vector3D position, intensity;

    type = v.second.get<std::string>("<xmlattr>.type");

    buf.str( v.second.get<std::string>("position") );
    buf >> position;
    buf.clear();

    buf.str( v.second.get<std::string>("intensity") );
    buf >> intensity;
    buf.clear();

    lightData ldata;
    ldata.position = position;
    ldata.intensity = intensity;

    if (type == "point") {
        ldata.type = point;
    }
    else if (type == "area") {

      sivelab::Vector3D normal;
      buf.str( v.second.get<std::string>("normal") );
      buf >> normal;
      buf.clear();

      float width = 1.0, length = 1.0;
      width = v.second.get<float>("width");
      length = v.second.get<float>("length");

      ldata.normal = normal;
      ldata.width = width;
      ldata.length = length;
    }
    scene->addLight(ldata);
  }

  // 
  // Shader
  // 
  if (v.first == "shader") {
    scene->addShader(parseShaderData(v));
  }

  // 
  // Shape or Instance
  // 
  if (v.first == "shape" || v.first == "instance") {
    parseShapeData(v);
  }

  // Texture
  if (v.first == "texture") {
      std::cout << "Found texture, but not parsing yet..." << std::endl;
  }
}

void SceneFile_Instancer::parseShapeData( ptree::value_type const &v )
{
  bool notInstance = true;
  if (v.first == "instance")
    notInstance = false;
  // Shapes and instances are basically the same thing, but need to be
  // added to different structures within the scene
  std::string type, name;
  type = v.second.get<std::string>("<xmlattr>.type");
  name = v.second.get<std::string>("<xmlattr>.name");

  bool useWireframe = false;
  // Can optionally have a transform associated with it...
  //
  // Need to find the transform in the second part of the shape pair
  Matrix4x4 m;
  ptree::const_assoc_iterator it = v.second.find("transform");
  if( it != v.second.not_found() )
  {
    m = parseTransformData( *it );
  }
  else
  {
    m = Matrix4x4(1,0,0,0,
                  0,1,0,0,
                  0,0,1,0,
                  0,0,0,1);
  }
  // Make sure to find the shader data objects; HAVE to include a
  // shader in this version; Need to find the shader in the second part
  // of the shape pair

  // We need a shader to render at the moment!
  //assert(shaderPtr);

    shaderData *shaderPtr = 0;
    it = v.second.find("shader");
    if( it != v.second.not_found() )
      {
        shaderPtr = parseShaderData( *it );
      }

  shapeData shape;
  std::istringstream buf;

    if (type == "instance") 
    {
      // check for the instance id
      std::string id = v.second.get<std::string>("<xmlattr>.id");
      // locate instance in map
      shape.type = Sinstance;
      shape.name = name;
      shape.m = m;
      if (shaderPtr == 0)
      {
        shape.shader.name = "";
      }
      else
        shape.shader = *shaderPtr;
      shape.id = id;
      scene->addModel(shape);
    }
    else if (type == "sphere") {
      float radius;
      sivelab::Vector3D center;

      buf.str( v.second.get<std::string>("center") );
      buf >> center;
      buf.clear();

      radius = v.second.get<float>("radius");

      shape.type = sphere;
      shape.name = name;
      shape.radius = radius;
      shape.center = center;
      shape.shader = *shaderPtr;

      std::cout << "\tFound sphere!" << std::endl;

      if (notInstance)
        scene->addModel(shape);
      else
        scene->addInstance(shape);
    }

    else if (type == "box") {

      sivelab::Vector3D minPt, maxPt;

      buf.str( v.second.get<std::string>("minPt") );
      buf >> minPt;
      buf.clear();

      buf.str( v.second.get<std::string>("maxPt") );
      buf >> maxPt;
      buf.clear();

      shape.type = box;
      shape.name = name;
      shape.minPt = minPt;
      shape.maxPt = maxPt;
      shape.shader = *shaderPtr;

      std::cout << "\tFound box!" << std::endl;
      if (notInstance)
        scene->addModel(shape);
      else
        scene->addInstance(shape);
    }

    else if (type == "triangle") {
      sivelab::Vector3D v0, v1, v2;

      buf.str( v.second.get<std::string>("v0") );
      buf >> v0;
      buf.clear();

      buf.str( v.second.get<std::string>("v1") );
      buf >> v1;
      buf.clear();

      buf.str( v.second.get<std::string>("v2") );
      buf >> v2;
      buf.clear();

      shape.type = triangle;
      shape.name = name;
      shape.v0 = v0;
      shape.v1 = v1;
      shape.v2 = v2;
      shape.shader = *shaderPtr;

      std::cout << "\tFound triangle!" << std::endl;
      if (notInstance)
        scene->addModel(shape);
      else
        scene->addInstance(shape);
    }

    else if (type == "mesh") 
    {
      std::string mesh_filename = v.second.get<std::string>("file");
      shape.file = mesh_filename;
      shape.shader = *shaderPtr;
      shape.type = mesh;
      shape.name = name;

      if (notInstance)
        scene->addModel(shape);
      else
        scene->addInstance(shape);
    }
  }


shaderData* SceneFile_Instancer::parseShaderData( ptree::value_type const &v )
{
  std::string type, name;
  shaderData* shaderPtr_toReturn = 0;

  std::istringstream buf;
  boost::optional<std::string> optShaderRef = v.second.get_optional<std::string>("<xmlattr>.ref");

  if (!optShaderRef) {
    type = v.second.get<std::string>("<xmlattr>.type");
    name = v.second.get<std::string>("<xmlattr>.name");
  }

  // if name exists in the shader map and this is a ref, return the shader pointer
  // otherwise, add the shader if it is NOT a ref and the name doesn't exist
  // final is to throw error

  std::map<std::string, shaderData*>::const_iterator mIterator = shaderMap.find(name);
  if (mIterator == shaderMap.end() && !optShaderRef) {

    // Did not find the shader and it was not a reference, so create and return
    if (type == "Lambertian") {
      sivelab::Vector3D kd;
      buf.str( v.second.get<std::string>("diffuse") );
      buf >> kd;
      buf.clear();

      shaderPtr_toReturn = new shaderData();
      shaderPtr_toReturn->type = lambertian;
      shaderPtr_toReturn->kd_diffuse = kd;

    }
    else if (type == "BlinnPhong" || type == "Phong") {
      sivelab::Vector3D d, s;
      float phongExp;

      buf.str( v.second.get<std::string>("diffuse") );
      buf >> d;
      buf.clear();

      buf.str( v.second.get<std::string>("specular") );
      buf >> s;
      buf.clear();

      phongExp = v.second.get<float>("phongExp");

      shaderPtr_toReturn = new shaderData();

      shaderPtr_toReturn->kd_diffuse = d;
      shaderPtr_toReturn->ks_specular = s;
      shaderPtr_toReturn->phongExp = phongExp;

      if (type == "BlinnPhong") {
          shaderPtr_toReturn->type = blinnphong;
      }
      else {
          shaderPtr_toReturn->type = phong;
      }
    }

    else if (type == "Mirror") {
        shaderPtr_toReturn = new shaderData();
        shaderPtr_toReturn->type = mirror;
    }
    else if (type == "Glaze")
    {
      sivelab::Vector3D kd;
      float re;
      buf.str( v.second.get<std::string>("diffuse") );
      buf >> kd;
      buf.clear();

      buf.str(v.second.get<std::string>("mirrorCoef") );
      buf >> re;
      buf.clear();

      shaderPtr_toReturn = new shaderData();
      shaderPtr_toReturn->type = glaze;
      shaderPtr_toReturn->kd_diffuse = kd;
      shaderPtr_toReturn->mirrorCoef =re;
    }
    else if (type == "BlinnPhongMirrored")
    {
      sivelab::Vector3D d, s;
      float phongExp, re;

      buf.str( v.second.get<std::string>("diffuse") );
      buf >> d;
      buf.clear();

      buf.str( v.second.get<std::string>("specular") );
      buf >> s;
      buf.clear();

      phongExp = v.second.get<float>("phongExp");

      buf.str(v.second.get<std::string>("mirrorCoef") );
      buf >> re;
      buf.clear();

      shaderPtr_toReturn = new shaderData();

      shaderPtr_toReturn->kd_diffuse = d;
      shaderPtr_toReturn->ks_specular = s;
      shaderPtr_toReturn->phongExp = phongExp;
      shaderPtr_toReturn->mirrorCoef =re;
      shaderPtr_toReturn->type = blinnPhongMirrored;

    }
    else if (type == "Dielectric")
    {
      sivelab::Vector3D attenuationCoef(1.0f,1.0f,1.0f);
      float rI;
      ptree::const_assoc_iterator aC = v.second.find("attenuationCoef");
      if (aC != v.second.not_found())
      {
        buf.str(v.second.get<std::string>("attenuationCoef") );
        buf >> attenuationCoef;
        buf.clear();
      }
        
      buf.str(v.second.get<std::string>("refractiveIndex") );
      buf >> rI;
      buf.clear();

      shaderPtr_toReturn = new shaderData();
      shaderPtr_toReturn->type = dielectric;
      shaderPtr_toReturn->refractiveIndex = rI;
      shaderPtr_toReturn->attenuationCoef = attenuationCoef;
    }
     
    shaderPtr_toReturn->name = name;
    shaderMap[name] = shaderPtr_toReturn;
    
    return shaderPtr_toReturn;
  }
    

  
  else if (optShaderRef) {
    
    // optShaderRef is true, so this should be a shader reference.
    // Attempt to find the shader in the map and return it.
    std::map<std::string, shaderData*>::const_iterator mIterator = shaderMap.find( *optShaderRef );
    if (mIterator != shaderMap.end())
      return mIterator->second;
    else 
      return 0;
  }
  else 
    std::cerr << "Error that should be caught!" << std::endl;
  
  return 0;
}


Matrix4x4 SceneFile_Instancer::parseTransformData( ptree::value_type const &v )
{
std::string name;
name = v.second.get<std::string>("<xmlattr>.name");
Matrix4x4 m, tmpM;
m.setIdentity();
std::stack<Matrix4x4> transformStack;
std::istringstream buf;
ptree::const_iterator nodeIterator;
for (nodeIterator=v.second.begin(); nodeIterator!=v.second.end(); 
++nodeIterator)
{
tmpM.setIdentity();
if (nodeIterator->first == "translate") {
// From stackoverflow:
// http://stackoverflow.com/questions/4597048/boost-property-tree-
//iterators-how-to-handle-them
//
// The iterator returns a value_type object which resembles a
// pair, and has the first and second accessors. v.first is an
// std::string holding the parent node (in my case "object1",
// "object2"), and v.second is a boost::property_tree::ptree,
// which can be used to parse the fields of the object.
Vector3D trans;
buf.str( nodeIterator->second.get_value<std::string>() );
buf >> trans;
buf.clear();
tmpM.makeTranslation(trans[0], trans[1], trans[2]);
transformStack.push(tmpM);
}
else if (nodeIterator->first == "rotate") {
float rot;
buf.str( nodeIterator->second.get_value<std::string>() );
buf >> rot;
buf.clear();
std::string axis = 
nodeIterator->second.get<std::string>("<xmlattr>.axis");
if (axis == "X") {
  tmpM.makeRotationX(rot * M_PI/180.0);
  transformStack.push(tmpM);
} else if (axis == "Y") {
  tmpM.makeRotationY(rot * M_PI/180.0);
  transformStack.push(tmpM);
} else if (axis == "Z") {
  tmpM.makeRotationZ(rot * M_PI/180.0);
  transformStack.push(tmpM);
}
}
else if (nodeIterator->first == "scale") {
sivelab::Vector3D scale;
buf.str( nodeIterator->second.get_value<std::string>() );
buf >> scale;
buf.clear();
tmpM.makeScale(scale[0], scale[1], scale[2]);
transformStack.push(tmpM);
}
}
//   std::cout << "New Transform\n-----------------" << std::endl;
std::list<Matrix4x4>::const_iterator mIter;
while (!transformStack.empty()) {
Matrix4x4 m1 = transformStack.top();
transformStack.pop();
// std::cout << "M1 = " << std::endl;
// m1.print();
// std::cout << std::endl;
m = m1 * m;
}
return m;
}