#include "Mirror.h"

png::rgb_pixel Mirror::getColor(const Vector3D e, const Vector3D intersect, const Vector3D normal, const std::vector<Light*> lights, const std::vector<int> hitLights, png::rgb_pixel reflect) const
{

	return reflect;

}

png::rgb_pixel Mirror::getFragmentColor(glm::vec4 normal, std::vector<glm::vec3> lightDirs, std::vector<glm::vec3> halves, std::vector<Light*> lights) const
{
	return png::rgb_pixel(0,0,0);
}