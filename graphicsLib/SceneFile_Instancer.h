#ifndef __SCENE_INSTANCER_H__
#define __SCENE_INSTANCER_H__ 1

#include <string>
#include <map>
#include <stack>

#include "XMLSceneParser.h"
#include "handleGraphicsArgs.h"
#include "Vector3D.h"
#include "Matrix4x4.h"

#include "SceneDataTypes.h"
#include "Scene.h"

using boost::property_tree::ptree;

class SceneFile_Instancer : public sivelab::SceneElementCreator
{
public:

    SceneFile_Instancer(Scene * newScene) {scene = newScene;}
    ~SceneFile_Instancer() {}

  // Derived class functions
  void instance( ptree::value_type const &v ); 

private:
	Scene * scene;

    void parseShapeData( ptree::value_type const &v );
    shaderData* parseShaderData( ptree::value_type const &v );
    
    // std::map<std::string, Texture*> textureSources;
    std::map<std::string, shaderData*> shaderMap;

    Matrix4x4 parseTransformData( ptree::value_type const &v );

};

#endif
